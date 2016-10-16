#ifdef TARGETPYTHON
#include "Python.h"
#include <stdint.h>
#endif

#ifndef TARGETPYTHON
#define STRICT_R_HEADERS
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include <Rinternals.h>
#endif

/**
 * @brief Generate a single random number using the capped Tausworthe RNG
 *
 * @details
 * This generates random numbers according to the process described in [1]. As 
 * an additional step, the resulting random number is capped to 0xFFFFFFFF 
 * using a bitwise and. This is done to yield the range [0, 2^32-1]. On 
 * return, the state variables are updated.
 *
 * [1]: @article{l1996maximally,
 *   title={Maximally equidistributed combined Tausworthe generators},
 *   author={L’ecuyer, Pierre},
 *   journal={Mathematics of Computation of the American Mathematical 
 *   Society},
 *   volume={65},
 *   number={213},
 *   pages={203--213},
 *   year={1996}
 *   }
 *
 * @param[in,out] state 	pointer to current state array
 *
 * @return a generated random number
 */
uint64_t lfsr113(uint64_t **state)
{
	uint64_t z1, z2, z3, z4, b;

	z1 = (*state)[0];
	z2 = (*state)[1];
	z3 = (*state)[2];
	z4 = (*state)[3];

	b = (((z1 << 6) ^ z1) >> 13);
	z1 = (((z1 & 4294967294) << 18) ^ b);

	b = (((z2 << 2) ^ z2) >> 27);
	z2 = (((z2 & 4294967288) << 2) ^ b);

	b = (((z3 << 13) ^ z3) >> 21);
	z3 = (((z3 & 4294967280) << 7) ^ b);

	b = (((z4 << 3) ^ z4) >> 12);
	z4 = (((z4 & 4294967168) << 13) ^ b);

	b = (z1 ^ z2 ^ z3 ^ z4);

	(*state)[0] = z1;
	(*state)[1] = z2;
	(*state)[2] = z3;
	(*state)[3] = z4;

	b = b & 0xFFFFFFFF;

	return(b);
}

/**
 * @brief Seed the Tausworthe RNG using a seed value
 *
 * @details
 * This function seeds the state array using a supplied seed value. As noted 
 * in [1] (see lfsr113()), the values of z1, z2, z3, and z4 should be larger 
 * than 1, 7, 15, and 127 respectively.
 *
 * @param[in] seed 	user supplied seed value for the RNG
 * @param[out] state  	state of the RNG
 */
void lfsr113_seed(uint64_t seed, uint64_t **state)
{
	uint64_t z1 = 2,
		      z2 = 8,
		      z3 = 16,
		      z4 = 128;

	z1 = (z1 * (seed + 1));
	z2 = (z2 * (seed + 1));
	z3 = (z3 * (seed + 1));
	z4 = (z4 * (seed + 1));

	z1 = (z1 > 1) ? z1 : z1 + 1;
	z2 = (z2 > 7) ? z2 : z2 + 7;
	z3 = (z3 > 15) ? z3 : z3 + 15;
	z4 = (z4 > 127) ? z4 : z4 + 127;

	if (*state == NULL) {
		(*state) = malloc(sizeof(uint64_t)*4);
	}

	(*state)[0] = z1;
	(*state)[1] = z2;
	(*state)[2] = z3;
	(*state)[3] = z4;
}

#ifdef TARGETPYTHON
/*
 *
 * Start of Python code
 *
 */

static PyObject *syncrng_seed(PyObject *self, PyObject *args)
{
	uint64_t seed, *state = NULL;

	if (!PyArg_ParseTuple(args, "k", &seed))
		return NULL;

	lfsr113_seed(seed, &state);

	PyObject *pystate = Py_BuildValue("[k, k, k, k]",
		       	state[0], state[1], state[2], state[3]);
	free(state);
	return pystate;
}

static PyObject *syncrng_rand(PyObject *self, PyObject *args)
{
	uint64_t i, value, numints, *localstate;

	PyObject *listObj;
	PyObject *intObj;

	if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &listObj))
		return NULL;

	// we're just assuming you would never pass more than 4 values
	localstate = malloc(sizeof(uint64_t)*5);
	numints = PyList_Size(listObj);
	for (i=0; i<numints; i++) {
		intObj = PyList_GetItem(listObj, i);
		value = (uint64_t) PyLong_AsLong(intObj);
		localstate[i] = value;
	}

	uint64_t rand = lfsr113(&localstate);
	localstate[4] = rand;

	PyObject *pystate = Py_BuildValue("[k, k, k, k, k]",
		       	localstate[0], localstate[1], localstate[2],
			localstate[3], rand);
	free(localstate);
	return pystate;
}

static PyMethodDef SyncRNGMethods[] = {
	{"seed", syncrng_seed, METH_VARARGS,
		"Seed the RNG."},
	{"rand", syncrng_rand, METH_VARARGS,
		"Generate a single random integer using SyncRNG."},
	{NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
	static struct PyModuleDef moduledef = {
		PyModuleDef_HEAD_INIT,
		"syncrng",
		"Python interface to SyncRNG",
		-1,
		SyncRNGMethods,
		NULL,
		NULL,
		NULL,
		NULL
	};
#endif


static PyObject *
moduleinit(void)
{
	PyObject *m;

	#if PY_MAJOR_VERSION >= 3
	m = PyModule_Create(&moduledef);
	#else
	m = Py_InitModule3("syncrng", SyncRNGMethods,
			"Python interface to SyncRNG");
	#endif

	return m;
}

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC
PyInit_syncrng(void)
{
	return moduleinit();
}
#else
PyMODINIT_FUNC
initsyncrng(void)
{
	moduleinit();
}
#endif
#endif

#ifndef TARGETPYTHON
/*
 *
 * Start of R code
 *
 */
SEXP R_syncrng_seed(SEXP seed)
{
	int i;
	double *pseed = REAL(seed),
	       *pstate = NULL;
	uint64_t useed = (uint64_t) *pseed;
	uint64_t *state = NULL;

	lfsr113_seed(useed, &state);

	SEXP Rstate = PROTECT(allocVector(REALSXP, 5));
	pstate = REAL(Rstate);
	for (i=0; i<4; i++) {
		pstate[i] = (double) state[i];
	}
	pstate[4] = -1.0;
	free(state);

	UNPROTECT(1);
	return Rstate;
}

SEXP R_syncrng_rand(SEXP state)
{
	uint64_t *localstate = malloc(sizeof(uint64_t)*4);
	double *pstate = REAL(state);
	int i;
	for (i=0; i<4; i++) {
		localstate[i] = (uint64_t) pstate[i];
	}

	uint64_t rand = lfsr113(&localstate);

	SEXP Rstate = PROTECT(allocVector(REALSXP, 5));
	pstate = REAL(Rstate);

	for (i=0; i<4; i++) {
		pstate[i] = (double) localstate[i];
	}
	pstate[4] = (double) rand;
	UNPROTECT(1);

	free(localstate);

	return Rstate;
}
/*
 *
 * End of R code
 *
 */
#endif
