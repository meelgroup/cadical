#ifndef _ipasir_h_INCLUDED
#define _ipasir_h_INCLUDED

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "cadical_export.hpp"
/*------------------------------------------------------------------------*/

// Here are the declarations for the actual IPASIR functions, which is the
// generic incremental reentrant SAT solver API used for instance in the SAT
// competition.  The other 'C' API in 'ccadical.h' is (more) type safe and
// has additional functions only supported by the CaDiCaL library.  Please
// also refer to our SAT Race 2015 article in the Journal of AI from 2016.

CADICAL_API const char *ipasir_signature (void);
CADICAL_API void *ipasir_init (void);
CADICAL_API void ipasir_release (void *solver);
CADICAL_API void ipasir_add (void *solver, int lit);
CADICAL_API void ipasir_assume (void *solver, int lit);
CADICAL_API int ipasir_solve (void *solver);
CADICAL_API int ipasir_val (void *solver, int lit);
CADICAL_API int ipasir_failed (void *solver, int lit);

CADICAL_API void ipasir_set_terminate (void *solver, void *state,
                           int (*terminate) (void *state));

CADICAL_API void ipasir_set_learn (void *solver, void *state, int max_length,
                       void (*learn) (void *state, int *clause));

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/

#endif
