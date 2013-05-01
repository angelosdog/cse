//
// Builtin functions.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//

#include "builtins.h"
#include "environment.h"
#include "eval.h"
#include "parser.h"
#include "symbols.h"
#include "types.h"
#include "values.h"

// The list data constructors.
char *listEmptyTag = "[]";
char *listConsTag = ":";

// Built-in types. Slightly nasty, we need to initialise the union somehow.
type_t boolT = {t_bool, {{NULL, NULL}}};
type_t charT = {t_char, {{NULL, NULL}}};
type_t numT  = {t_num,  {{NULL, NULL}}};

static int add_type_binding_i(void *data, void *info)
{
  symboltable_t *typeMap = (symboltable_t *)info;
  type_t *t = (type_t *)data;

  st_insert(typeMap, user_def_datacons_name(t), user_def_datacons_dcs(t));

  return 1;
}

symboltable_t *initial_type_environment(list_t *extraTypes)
{
  symboltable_t *typeMap = st_init();

  st_insert(typeMap, "Bool", &boolT);
  st_insert(typeMap, "Char", &charT);
  st_insert(typeMap, "Num",  &numT);

  list_iterate(extraTypes, add_type_binding_i, typeMap);

  return typeMap;
}

//**************************************

// A pseudo-random sequence generator.
// This technique (linear congruential generation) cannot be (easily)
// implemented in FLan as it does not use twos-complement arithmetic.
// http://en.wikipedia.org/wiki/Linear_congruential_generator
//
static value_t *flan_random_fn(list_t *args)
{
  value_t *seed = (value_t *)thunk_force(list_head(args));

  unsigned long long a = 1103515245;
  unsigned long long c = 12345;
  unsigned long long s = num_val(seed);
  unsigned long long m = 4294967296; // 2 ^ 32
  unsigned long long r = (a * s + c) % m;

  value_t *result;

  // Trim down the number of bits we use.
  result = alloc_value(v_num);
  num_val(result) = r >> 12;

  return result;
}

env_t *builtin_bindings(void)
{
  env_t *env;
  value_t *v;

  env = env_init();

  v = alloc_value(v_builtin_fn);
  builtin_num_params(v) = 1;
  builtin_args(v) = list_empty();
  builtin_fn(v) = flan_random_fn;
  env_add_binding(env, "random", v);

  return env;
}

env_t *builtin_bindings_types(void)
{
  env_t *env;
  type_t *t;

  env = env_init();

  t = alloc_type(t_fn);
  fn_arg_type(t) = &numT;
  fn_ret_type(t) = &numT;
  env_add_binding(env, "random", t);


  return env;
}
