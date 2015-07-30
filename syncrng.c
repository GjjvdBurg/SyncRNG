#ifdef TARGETPYTHON
#include "Python.h"
#endif

#ifndef TARGETPYTHON
#define STRICT_R_HEADERS
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
 * an additional step, the state variables are capped to 0x7FFFFFFF using a 
 * bitwise and. This is to overcome limitations of R. On return, the state 
 * variables are updated.
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
int lfsr113(int **state)
{
	unsigned long z1, z2, z3, z4, b;

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

	(*state)[0] = z1 & 0x7FFFFFFF;
	(*state)[1] = z2 & 0x7FFFFFFF;
	(*state)[2] = z3 & 0x7FFFFFFF;
	(*state)[3] = z4 & 0x7FFFFFFF;
	b = b & 0x7FFFFFFF;
	
	return(b);
}

/**
 * @brief Seed the Tausworthe RNG using a seed value
 *
 * @details
 * This function seeds the state array using a supplied seed value. As noted 
 * in [1] (see lfsr113()), the values of z1, z2, z3, and z4 should be larger 
 * than 1, 7, 15, and 127 respectively. Here too the state variables are 
 * capped at 0x7FFFFFF.
 *
 * @param[in] seed 	user supplied seed value for the RNG
 * @param[out] state  	state of the RNG
 */
void lfsr113_seed(unsigned long seed, int **state)
{
	unsigned long z1 = 2,
		      z2 = 8,
		      z3 = 16,
		      z4 = 128;

	z1 = (z1 * (seed + 1)) & 0x7FFFFFFF;
	z2 = (z2 * (seed + 1)) & 0x7FFFFFFF;
	z3 = (z3 * (seed + 1)) & 0x7FFFFFFF;
	z4 = (z4 * (seed + 1)) & 0x7FFFFFFF;

	if (*state == NULL) {
		(*state) = malloc(sizeof(int)*4);
	}

	(*state)[0] = (int) z1;
	(*state)[1] = (int) z2;
	(*state)[2] = (int) z3;
	(*state)[3] = (int) z4;
}

#ifdef TARGETPYTHON
/*
 *
 * Start of Python code
 *
 */

static PyObject *syncrng_seed(PyObject *self, PyObject *args)
{
	int seed, *state = NULL;

	if (!PyArg_ParseTuple(args, "i", &seed))
		return NULL;

	lfsr113_seed(seed, &state);
	PyObject *pystate = Py_BuildValue("[i, i, i, i]",
		       	state[0], state[1], state[2], state[3]);
	free(state);
	return pystate;
}

static PyObject *syncrng_rand(PyObject *self, PyObject *args)
{
	int i, value, numints, *localstate;

	PyObject *listObj;
	PyObject *intObj;

	if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &listObj))
		return NULL;

	// we're just assuming you would never pass more than 4 values
	localstate = malloc(sizeof(int)*5);
	numints = PyList_Size(listObj);
	for (i=0; i<numints; i++) {
		intObj = PyList_GetItem(listObj, i);
		value = (int) PyLong_AsLong(intObj);
		localstate[i] = value;
	}

	int rand = lfsr113(&localstate);
	localstate[4] = rand;

	PyObject *pystate = Py_BuildValue("[i, i, i, i, i]",
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

PyMODINIT_FUNC initsyncrng(void)
{
	PyObject *m = Py_InitModule3("syncrng", SyncRNGMethods,
		       	"Python interface to SyncRNG");
	if (m == NULL)
		return;
}
#endif

#ifndef TARGETPYTHON
/*
 *
 * Start of R code
 *
 */
SEXP R_syncrng_seed(SEXP seed)
{
	int i, *pstate = NULL, *state = NULL;
	int *pseed = INTEGER(seed);

	SEXP Rstate = PROTECT(allocVector(INTSXP, 5));
	pstate = INTEGER(Rstate);

	lfsr113_seed(*pseed, &state);

	for (i=0; i<4; i++) {
		pstate[i] = state[i];
	}
	pstate[4] = -1;
	free(state);

	UNPROTECT(1);
	return Rstate;
}

SEXP R_syncrng_rand(SEXP state)
{
	int *localstate = malloc(sizeof(int)*4);
	int *pstate = INTEGER(state);
	int i;
	for (i=0; i<4; i++)
		localstate[i] = pstate[i];

	int rand = lfsr113(&localstate);
	
	SEXP Rstate = PROTECT(allocVector(INTSXP, 5));
	pstate = INTEGER(Rstate);

	for (i=0; i<4; i++)
		pstate[i] = localstate[i];
	pstate[4] = rand;
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
