#define STRICT_R_HEADERS
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Random.h>
#include <R_ext/Rdynload.h>

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
uint32_t lfsr113(uint64_t **state)
{
	// NOTE: This function *must* be the same as in the Python library
	uint64_t z1, z2, z3, z4;
	uint64_t b;

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

	return((uint32_t) b);
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
void lfsr113_seed(uint32_t seed, uint64_t **state)
{
	// NOTE: This function *must* be the same as in the Python library
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

/*
 *
 * Start of R code
 *
 */

SEXP R_syncrng_seed(SEXP seed);
SEXP R_syncrng_rand(SEXP state);

R_CallMethodDef callMethods[] = {
	{"R_syncrng_seed", (DL_FUNC) &R_syncrng_seed, 1},
	{"R_syncrng_rand", (DL_FUNC) &R_syncrng_rand, 1},
	{NULL, NULL, 0}
};
R_CMethodDef cMethods[] = {
	{NULL, NULL, 0}
};

void R_init_myLib(DllInfo *info)
{
	R_registerRoutines(info, cMethods, callMethods, NULL, NULL);
	R_useDynamicSymbols(info, TRUE);
}

// Set the seed for the generator from the reference class
SEXP R_syncrng_seed(SEXP seed)
{
	int i;
	double *pseed = REAL(seed),
	       *pstate = NULL;
	uint32_t useed = (uint32_t) *pseed;
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

// get a random number from the reference class
SEXP R_syncrng_rand(SEXP state)
{
	uint64_t *localstate = malloc(sizeof(uint64_t)*4);
	double *pstate = REAL(state);
	int i;
	for (i=0; i<4; i++) {
		localstate[i] = (uint64_t) pstate[i];
	}

	uint32_t rand = lfsr113(&localstate);

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
 * The following code is used to make SyncRNG a real "user-defined" RNG 
 * following .Random.user documentation.
 *
 */
static uint32_t global_R_seed;
static int global_R_nseed = 1;
static double global_R_result_uniform;
static double global_R_result_normal;
static uint64_t *global_R_state = NULL;

double *user_unif_rand()
{
	if (global_R_state == NULL) {
		// if it's not seeded yet we seed it with 0
		global_R_seed = 0;
		lfsr113_seed(global_R_seed, &global_R_state);
	}

	uint32_t rand = lfsr113(&global_R_state);
	global_R_result_uniform = rand * 2.3283064365387e-10;
	return &global_R_result_uniform;
}

// see: https://stackoverflow.com/q/47824450/1154005
Int32 _unscramble(Int32 scram)
{
	int j;
	uint32_t temp = scram;
	uint32_t mmi = 2783094533;
	for (j=0; j<50; j++) {
		temp = ((temp - 1) * mmi);
	}
	return ((Int32) temp);
}

// note that Int32 is "unsigned int" which is not necessarily 32 bit
void user_unif_init(Int32 seed_in)
{
	global_R_seed = seed_in;
	uint32_t useed = _unscramble(seed_in);

	// destroy the previous state, we're reseeding the RNG
	if (global_R_state != NULL) {
		free(global_R_state);
		global_R_state = NULL;
	}
	lfsr113_seed(useed, &global_R_state);
}

int *user_unif_nseed()
{
	return &global_R_nseed;
}

int *user_unif_seedloc()
{
	return (int *) &global_R_seed;
}

double *user_norm_rand()
{
	double u, v, z, x;
	do {
		u = *user_unif_rand();
		v = 0.857764 * (2. * (*user_unif_rand()) - 1);
		x = v/u;
		z = 0.25 * x * x;
		if (z < 1. - u)
			break;
		if (z > 0.259/u + 0.35)
			continue;
	} while (z > -log(u));
	global_R_result_normal = x;
	return &global_R_result_normal;
}

/*
 *
 * End of R code
 *
 */
