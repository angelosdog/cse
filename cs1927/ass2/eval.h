//
// The evaluators.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//

#ifndef _EVAL_H_
#define _EVAL_H_

#include "environment.h"
#include "parser.h"
#include "values.h"

// First-order strict.
value_t *evaluate_program_fos(env_t *env, program_t *program);

// Higher-order strict.
value_t *evaluate_program_hos(env_t *env, program_t *program);

// Higher-order lazy.
value_t *evaluate_program_hol(env_t *env, program_t *program);

// We may need to force thunks while printing values.
value_t *thunk_force(value_t *v);

#endif // _EVAL_H_
