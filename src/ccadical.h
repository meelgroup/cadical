#ifndef _ccadical_h_INCLUDED
#define _ccadical_h_INCLUDED

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

#include "cadical_export.hpp"

// C wrapper for CaDiCaL's C++ API following IPASIR.

typedef struct CCaDiCaL CCaDiCaL;

CADICAL_API const char *ccadical_signature (void);
CADICAL_API CCaDiCaL *ccadical_init (void);
CADICAL_API void ccadical_release (CCaDiCaL *);

CADICAL_API void ccadical_add (CCaDiCaL *, int lit);
CADICAL_API void ccadical_assume (CCaDiCaL *, int lit);
CADICAL_API int ccadical_solve (CCaDiCaL *);
CADICAL_API int ccadical_val (CCaDiCaL *, int lit);
CADICAL_API int ccadical_failed (CCaDiCaL *, int lit);

CADICAL_API void ccadical_set_terminate (CCaDiCaL *, void *state,
                             int (*terminate) (void *state));

CADICAL_API void ccadical_set_learn (CCaDiCaL *, void *state, int max_length,
                         void (*learn) (void *state, int *clause));

/*------------------------------------------------------------------------*/

// Non-IPASIR conformant 'C' functions.

CADICAL_API void ccadical_constrain (CCaDiCaL *, int lit);
CADICAL_API int ccadical_constraint_failed (CCaDiCaL *);
CADICAL_API void ccadical_set_option (CCaDiCaL *, const char *name, int val);
CADICAL_API void ccadical_limit (CCaDiCaL *, const char *name, int limit);
CADICAL_API int ccadical_get_option (CCaDiCaL *, const char *name);
CADICAL_API void ccadical_print_statistics (CCaDiCaL *);
CADICAL_API int64_t ccadical_active (CCaDiCaL *);
CADICAL_API int64_t ccadical_irredundant (CCaDiCaL *);
CADICAL_API int ccadical_fixed (CCaDiCaL *, int lit);
CADICAL_API int ccadical_trace_proof (CCaDiCaL *, FILE *, const char *);
CADICAL_API void ccadical_close_proof (CCaDiCaL *);
CADICAL_API void ccadical_conclude (CCaDiCaL *);
CADICAL_API void ccadical_terminate (CCaDiCaL *);
CADICAL_API void ccadical_freeze (CCaDiCaL *, int lit);
CADICAL_API int ccadical_frozen (CCaDiCaL *, int lit);
CADICAL_API void ccadical_melt (CCaDiCaL *, int lit);
CADICAL_API int ccadical_simplify (CCaDiCaL *);
CADICAL_API int ccadical_vars (CCaDiCaL *);
CADICAL_API int ccadical_declare_more_variables (CCaDiCaL *, int number_of_vars);
CADICAL_API int ccadical_declare_one_more_variable (CCaDiCaL *);
CADICAL_API void ccadical_phase (CCaDiCaL *wrapper, int lit);
CADICAL_API void ccadical_unphase (CCaDiCaL *wrapper, int lit);

/*------------------------------------------------------------------------*/

// Support legacy names used before moving to more IPASIR conforming names.

#define ccadical_reset ccadical_release
#define ccadical_sat ccadical_solve
#define ccadical_deref ccadical_val

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/

#endif
