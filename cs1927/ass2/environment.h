//
// Environments for the evaluator.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <stdio.h>

// An environment maps variables to their values. For typing, these
// will be (type_t *), for evaluation (value_t *).
typedef struct env env_t;

env_t *env_init(void);

// Creates a new scope, derived from an existing environment.  The
// original environment is not affected, but we overwrite the
// passed-in pointer to avoid some common errors.
env_t *env_new_scope(env_t **env);

// Adds a binding to the current scope. Returns 0 if this is a new
// binding, and 1 if there is an existing binding in the most-recent
// scope.
int env_add_binding(env_t *env, char *var, void *val);

void *env_lookup(env_t *env, char *var);

// Destroy the top-most scope and reclaim the storage.
void env_destroy_scope(env_t **env);

// Print the environment.
void env_print(FILE *outf, env_t *env, int (fn)(char *key, void *data, FILE *outf));

#endif // _ENVIRONMENT_H_
