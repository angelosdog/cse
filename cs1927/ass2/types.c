//
// Types, type checking.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 22/08/01 (SimpleC).
// Redone for FLAN August 2008.
//
//  - '[]' is a polymorphic constant. Need to take care checking lists.
//

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "environment.h"
#include "errors.h"
#include "parser.h"
#include "prettyprinter.h"
#include "symbols.h"
#include "types.h"
#include "util.h"

// SLIGHT HACK: the empty list '[]' is a polymorphic constant, and
// 't_undef' is used to stand for the arbitary list element type.
//
type_t undefT = {t_undef,  {{NULL, NULL}}};

// Have two environments:
//  - a global symbol table mapping type names to types.
//  - a local environment that maps variables to types (passed amongst functions).
//
static symboltable_t *typeMap;

type_t *alloc_type(type_type_t type)
{
  type_t *new;

  if ((new = (type_t *)util_malloc(sizeof(type_t))) == NULL) {
    error("alloc_type: out of memory.\n");
  }

  new->type = type;

  return new;
}

// static int print_type_env_i(char *name, void *data, void *info)
// {
//   fprintf(stdout, "%s :: ", name);
//   pp_type(stdout, (type_t *)data);

//   return 1;
// }

// static void print_type_env(symboltable_t *st)
// {
//   st_iterate(st, print_type_env_i, NULL);
// }

//*********************************************************
// Type Checking.

// A generic closure.
typedef struct {
  env_t *env;
  binding_t *binding;
  expr_t *expr;
  type_t *type;
  int failure;
} tc_closure_t;

static int eq_types(type_t *t1, type_t *t2);
static void tc_bindings(env_t *env, list_t *bs);

// How many errors have we seen so far?
static int errors = 0;

// Error on mismatched types.
static void tc_error_types(type_t *expected, type_t *got, expr_t *expr)
{
  fprintf(stdout, "Type mismatch.\n  Expected: ");
  pp_type(stdout, expected);
  fprintf(stdout, "\n       Got: ");
  pp_type(stdout, got);
  fprintf(stdout, "\n   on line %d: ", expr->line_num);
  pp_expr(stdout, expr, 2);
  fprintf(stdout, "\n");

  errors++;
}

static void tc_expect(type_t *expected, type_t *got, expr_t *expr)
{
  if (!eq_types(expected, got)) {
    tc_error_types(expected, got, expr);
  }
}

static void tc_expect_pat(type_t *expected, type_t *got, pattern_t *pat)
{
  if (!eq_types(expected, got)) {
    fprintf(stdout, "Type mismatch.\n  Expected: ");
    pp_type(stdout, expected);
    fprintf(stdout, "\n  Got: ");
    pp_type(stdout, got);
    fprintf(stdout, "\n   in pattern on line %d: ", pat->line_num);
    pp_pattern(stdout, pat);
    fprintf(stdout, "\n");

    errors++;
  }
}

static void tc_error(char *fmt, ...)
{
    va_list argp;

    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);

    errors++;
}

//**************************************

static int eq_types_i(void *l, void *r, void *info)
{
  type_t *t1 = (type_t *)l;
  type_t *t2 = (type_t *)r;
  int *result = (int *)info;

  return (*result = (eq_types(t1, t2)));
}

static int eq_types(type_t *t1, type_t *t2)
{
  int result = 0;

  // Handle the 't_undef' HACK: if either type is 't_undef', they're equal.
  if (t1->type == t_undef || t2->type == t_undef) {
    result = 1;
  } else if (t1->type == t2->type) {
    result = 1;

    switch (t1->type) {
    case t_undef:
    case t_unused:
      if (*(int *)NULL) {
        printf("should crash.\n");
      }
      break;

    case t_bool:
    case t_char:
    case t_num:
      break;

      // Check argument and return types for functions.
    case t_fn:
      result = eq_types(fn_arg_type(t1), fn_arg_type(t2))
            && eq_types(fn_ret_type(t1), fn_ret_type(t2));
      break;

      // Check base types for lists and tuples.
    case t_list:
      result = eq_types(list_elt_type(t1), list_elt_type(t2));
      break;
    
    case t_tuple:
      if (list_length(tuple_types(t1)) == list_length(tuple_types(t2))) {
        list_zip_with(tuple_types(t1), tuple_types(t2), eq_types_i, &result);
      } else {
        result = 0;
      }
      break;

    case t_user_defined:
      result = strcmp(user_def_use_name(t1), user_def_use_name(t2)) == 0;
      break;
    }
  }

  return result;
}

// Verify a type is well-defined.
static int valid_type_i(void *data, void *info)
{
  type_t *type = (type_t *)data;

  switch (type->type) {
    // The magic undefined type is not valid in a user-given type signature.
  case t_undef:
  case t_unused:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    break;

    // Built-in types are OK.
  case t_bool:
  case t_char:
  case t_num:
    break;

    // Check argument and return types for functions.
  case t_fn:
    valid_type_i(fn_arg_type(type), info);
    valid_type_i(fn_ret_type(type), info);
    break;

    // Check base types for lists and tuples.
  case t_list:
    valid_type_i(list_elt_type(type), info);
    break;
    
  case t_tuple:
    list_iterate(tuple_types(type), valid_type_i, info);
    break;

    // A user-defined type must be in the environment.
  case t_user_defined:
    if (st_lookup(typeMap, user_def_use_name(type)) == NULL) {
      tc_error("Type '%s' is not defined.\n", user_def_use_name(type));
    }
  }

  return 1;
}

//**************************************

static type_t *tc_expr(env_t *env, expr_t *expr);

typedef struct {
  env_t *env;
  pattern_t *pat;
  type_t *type;
  type_t *retType;
  list_t *types;
} tce_closure_t;

static int tc_exprs_i(void *data, void *info)
{
  tce_closure_t *tce = (tce_closure_t *)info;
  expr_t *expr = (expr_t *)data;

  list_cons(tc_expr(tce->env, expr), tce->types);

  return 1;
}

static int tc_fncall_args_i(void *data, void *info)
{
  tc_closure_t *fnp = (tc_closure_t *)info;
  expr_t *arg = (expr_t *)data;
  type_t *t;

  t = tc_expr(fnp->env, arg);

  if (fnp->type->type == t_fn) {
    tc_expect(fn_arg_type(fnp->type), t, arg);
    fnp->type = fn_ret_type(fnp->type);

    return 1;
  } else {
    type_t type;

    // IMPROVE: This type error is a bit misleading, would be better to say "in arg i".
    type.type = t_fn;
    fn_arg_type(&type) = t;
    fn_ret_type(&type) = fnp->type;

    tc_expect(&type, t, fnp->expr);

    return 0;
  }
}

static type_t *tc_fncall(env_t *env, expr_t *expr)
{
  tc_closure_t fnp;

  fnp.env = env;
  fnp.type = tc_expr(env, fncall_fn(expr));
  fnp.expr = expr;
  list_iterate(fncall_args(expr), tc_fncall_args_i, &fnp);

  return fnp.type;
}

static type_t *tc_ite(env_t *env, expr_t *expr)
{
  type_t *tt;
  type_t *te;

  tc_expect(&boolT, tc_expr(env, ite_cond(expr)), ite_cond(expr));
  tt = tc_expr(env, ite_then(expr));
  te = tc_expr(env, ite_else(expr));

  if (!eq_types(tt, te)) {
    tc_error_types(tt, te, expr);
  }

  return tt;
}

static type_t *tc_let(env_t *env, expr_t *expr)
{
  type_t *result;

  env_new_scope(&env);
  tc_bindings(env, let_bindings(expr));
  result = tc_expr(env, let_body(expr));
  env_destroy_scope(&env);

  return result;
}

static type_t *tc_list_cons(env_t *env, expr_t *expr)
{
  type_t *tail;
  type_t *head;
  type_t *result;

  head = tc_expr(env, binary_left(expr));
  tail = tc_expr(env, binary_right(expr));

  // Assume the head type is correct.
  result = alloc_type(t_list);
  list_elt_type(result) = head;

  // If the tail is not a list, then it is clearly wrong.
  if (tail->type != t_list) {
    tc_expect(result, tail, binary_right(expr));
  } else {
    // Otherwise assume the user got the tail right and maybe the head wrong.
    tc_expect(list_elt_type(tail), head, binary_left(expr));
  }

  return result;
}

static int tc_pat_dc_i(void *data, void *info)
{
  tce_closure_t *tce = (tce_closure_t *)info;
  char *var = (char *)data;

  if (tce->type->type == t_fn) {
    env_add_binding(tce->env, var, fn_arg_type(tce->type));
    tce->type = fn_ret_type(tce->type);

    return 1;
  } else {
    tc_error("Too many arguments to data constructor '%s' on line %d.\n",
             datacons_tag(tce->pat), tce->pat->line_num);
    tce->type = &undefT;

    return 0;
  }
}

static void tc_pat_datacons(env_t *env, pattern_t *pat, type_t *type)
{
  type_t *dct;
  tce_closure_t tce;

  if ((dct = env_lookup(env, datacons_tag(pat))) == NULL) {
    tc_error("Unknown data constructor '%s'.\n", datacons_tag(pat));
  } else {
    tce.env = env;
    tce.pat = pat;
    tce.type = dct;
    list_iterate(datacons_params(pat),  tc_pat_dc_i, &tce);
    tc_expect_pat(tce.type, type, pat);
  }
}

static int add_var_binding_i(void *l, void *r, void *info)
{
  env_t *env = (env_t *)info;
  char *name = (char *)l;
  type_t *type = (type_t *)r;

  env_add_binding(env, name, type);

  return 1;
}

static void tc_pat_tuple(env_t *env, pattern_t *pat, type_t *type)
{
  if (type->type == t_tuple) {
    if (list_length(tuple_val(pat)) == list_length(tuple_types(type))) {
      // Bind pattern variables in the environment.
      list_zip_with(tuple_val(pat), tuple_types(type), add_var_binding_i, env);
    } else {
      tc_error("tc_pat_tuple: scrutinised tuple and the tuple pattern have different lengths.\n");
    }
  } else {
    tc_error("tc_pat_tuple: scrutinised value is not a tuple.\n");
  }
}

static void tc_pat_listcons(env_t *env, pattern_t *pat, type_t *type)
{
  if (type->type == t_list) {
    // Bind pattern variables in the environment.
    env_add_binding(env, listcons_hvar(pat), list_elt_type(type));
    env_add_binding(env, listcons_tvar(pat), type);
  } else {
    type_t list_type;

    list_type.type = t_list;
    list_elt_type(&list_type) = &undefT;

    tc_expect_pat(&list_type, type, pat);
  }
}

static int tc_listlit_i(void *data, void *info)
{
  tce_closure_t *tce = (tce_closure_t *)info;
  expr_t *expr = (expr_t *)data;
  type_t *t;

  t = tc_expr(tce->env, expr);
  if (tce->type == NULL) {
    tce->type = t;
  } else {
    tc_expect(tce->type, t, expr);
  }

  return 1;
}

static type_t *tc_listlit(env_t *env, expr_t *expr)
{
  tce_closure_t tce;
  type_t *t;

  tce.env = env;
  tce.type = NULL;
  list_iterate(list_val(expr), tc_listlit_i, &tce);

  t = alloc_type(t_list);
  list_elt_type(t) = tce.type;

  return t;
}

// We don't know the type of the list, so say it is undefined.
static type_t *tc_emptylist(env_t *env, expr_t *expr)
{
  type_t *t;

  t = alloc_type(t_list);
  list_elt_type(t) = &undefT;

  return t;
}

static void tc_pattern(env_t *env, pattern_t *pat, type_t *type)
{
  switch (pat->type) {
  case p_bconst:
    tc_expect_pat(type, &boolT, pat);
    break;
  case p_cconst:
    tc_expect_pat(type, &charT, pat);
    break;
  case p_datacons:
    tc_pat_datacons(env, pat, type);
    break;
  case p_nconst:
    tc_expect_pat(type, &numT, pat);
    break;
  case p_listempty:
    if (type->type != t_list) {
      // IMPROVE: if we had our hands on the expression...
      tc_error("tc_pattern/p_listempty: scrutinised expression does not have a list type.\n");
    }
    break;
  case p_listcons:
    tc_pat_listcons(env, pat, type);
    break;
  case p_tuple:
    tc_pat_tuple(env, pat, type);
    break;
  case p_var:
    env_add_binding(env, var_name(pat), type);
    break;
  default:
    error("INTERNAL tc_pattern: not a pattern.\n");
  }
}

static int tc_clauses_i(void *data, void *info)
{
  tce_closure_t *tce = (tce_closure_t *)info;
  clause_t *c = (clause_t *)data;
  type_t *t;

  env_new_scope(&tce->env);
  tc_pattern(tce->env, c->pattern, tce->type);
  t = tc_expr(tce->env, c->body);
  env_destroy_scope(&tce->env);

  if (tce->retType != NULL) {
    tc_expect(tce->retType, t, c->body);
  } else {
    tce->retType = t;
  }

  return 1;
}

static type_t *tc_match(env_t *env, expr_t *expr)
{
  tce_closure_t tce;

  tce.env = env;
  tce.type = tc_expr(env, match_scrutinee(expr));
  tce.retType = NULL;
  list_iterate(match_clauses(expr), tc_clauses_i, &tce);

  return tce.retType;
}

static type_t *tc_tuple(env_t *env, expr_t *expr)
{
  tce_closure_t tce;
  type_t *result;

  tce.env = env;
  tce.types = list_empty();
  list_iterate(tuple_val(expr), tc_exprs_i, &tce);
  list_reverse(tce.types);

  result = alloc_type(t_tuple);
  tuple_types(result) = tce.types;

  return result;
}

static type_t *tc_unaryop(env_t *env, expr_t *expr, type_t *type)
{
  type_t *t;

  t = tc_expr(env, unary_expr(expr));

  if (!eq_types(t, type)) {
    tc_error_types(type, t, unary_expr(expr));
  }

  return t;
}

// Allow equality testing on all types except functions.
static int equality_type(type_t *t)
{
  switch (t->type) {
  case t_unused:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    return -1;

  case t_fn:
    return 0;

  default:
    return 1;
  }
}

// Typecheck an equality expression.
static void tc_eqop(env_t *env, expr_t *expr)
{
  type_t *tl;
  type_t *tr;

  tl = tc_expr(env, binary_left(expr));
  tr = tc_expr(env, binary_right(expr));

  if (!eq_types(tl, tr)) {
    tc_error_types(tl, tr, expr);
  } else if (!equality_type(tl)) {
    tc_error("Equality test on line %d on an expression that does not support it.\n",
             expr->line_num);
  }
}

// Typecheck a binary expression. Both arguments are expected to have
// the given type.
static void tc_binaryop(env_t *env, expr_t *expr, type_t *type)
{
  type_t *tl;
  type_t *tr;

  tl = tc_expr(env, binary_left(expr));
  tr = tc_expr(env, binary_right(expr));

  if (!eq_types(tl, type)) {
    tc_error_types(type, tl, binary_left(expr));
  }

  if (!eq_types(tr, type)) {
    tc_error_types(type, tr, binary_right(expr));
  }
}

static type_t *tc_expr(env_t *env, expr_t *expr)
{
  type_t *result;

  switch (expr->type) {
  default: // This is to handle invalid tags.
  case p_unused:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    return NULL; // gcc dataflow

  case p_add:
  case p_div:
  case p_mod:
  case p_mul:
  case p_sub:
    tc_binaryop(env, expr, &numT);
    result = &numT;
    break;

  case p_eqop:
  case p_ne:
    tc_eqop(env, expr);
    result = &boolT;
    break;

  case p_ge:
  case p_gt:
  case p_le:
  case p_lt:
    tc_binaryop(env, expr, &numT);
    result = &boolT;
    break;

  case p_and:
  case p_or:
    tc_binaryop(env, expr, &boolT);
    result = &boolT;
    break;

  case p_negate:
    tc_unaryop(env, expr, &boolT);
    result = &boolT;
    break;

  case p_bconst:
    result = &boolT;
    break;
  case p_cconst:
    result = &charT;
    break;
  case p_datacons:
    result = env_lookup(env, var_name(expr));
    if (result == NULL) {
      tc_error("Data constructor '%s' not in scope on line %d.\n", var_name(expr), expr->line_num);
      // HACK: white lie
      result = &undefT;
    }
    break;
  case p_fncall:
    result = tc_fncall(env, expr);
    break;
  case p_nconst:
    result = &numT;
    break;
  case p_ite:
    result = tc_ite(env, expr);
    break;
  case p_let:
    result = tc_let(env, expr);
    break;
  case p_listcons:
    result = tc_list_cons(env, expr);
    break;
  case p_listlit:
    result = tc_listlit(env, expr);
    break;
  case p_listempty:
    result = tc_emptylist(env, expr);
    break;
  case p_match:
    result = tc_match(env, expr);
    break;

  case p_tuple:
    result = tc_tuple(env, expr);
    break;

  case p_var:
    result = env_lookup(env, var_name(expr));
    if (result == NULL) {
      tc_error("Variable '%s' not in scope on line %d.\n", var_name(expr), expr->line_num);
      // HACK: white lie
      result = &undefT;
    }
    break;
  }

  return result;
}

//**************************************

static int bind_fn_params_i(void *data, void *info)
{
  tc_closure_t *fnp = (tc_closure_t *)info;
  char *var = (char *)data;

  // Walk along the spine of the type tree.
  if (fnp->type->type == t_fn) {
    env_add_binding(fnp->env, var, fn_arg_type(fnp->type));
    fnp->type = fn_ret_type(fnp->type);

    return 1;
  } else {
    tc_error("Too many arguments for binding '%s' on line %d.\n",
             fnp->binding->name,
             fnp->binding->line_num); 
    errors++;

    return 0;
  }
}

static int tc_binding_i(void *data, void *info)
{
  env_t *env = (env_t *)info;
  binding_t *b = (binding_t *)data;
  tc_closure_t fnp;

//   fprintf(stdout, "type checking binding for %s\n", b->name);

  // Check the binding's type is plausible (all types are present in
  // the environment).
  valid_type_i(b->type, env);

  // In a new scope, add the types for all the binding's
  // parameters. As a side-effect, compute the return type of this
  // binding.
  env_new_scope(&env);
  fnp.env = env;
  fnp.binding = b;
  fnp.type = b->type;
  fnp.failure = 0;
  list_iterate(b->params, bind_fn_params_i, &fnp);

  // Type-check the body.
  tc_expect(fnp.type, tc_expr(env, b->body), b->body);

  env_destroy_scope(&env);

  return 1;
}

static int add_term_binding_i(void *data, void *info)
{
  env_t *env = (env_t *)info;
  binding_t *b = (binding_t *)data;

  env_add_binding(env, b->name, b->type);

  return 1;
}

static void tc_bindings(env_t *env, list_t *bs)
{
  // Add the types of all bindings to the environment.
  list_iterate(bs, add_term_binding_i, env);
  list_iterate(bs, tc_binding_i, env);
}

//**************************************

static int build_datacons_type_i(void *data, void *info)
{
  tce_closure_t *bdt = (tce_closure_t *)info;
  type_t *type = (type_t *)data;
  type_t *result;

  result = alloc_type(t_fn);
  fn_arg_type(result) = type;
  fn_ret_type(result) = bdt->type;

  bdt->type = result;

  return 1;
}

static int verify_datacons_def_i(void *data, void *info)
{
  tce_closure_t *tce = (tce_closure_t *)info;
  datacons_t *dc = (datacons_t *)data;

  // Verify the tag hasn't been used before.
  if (env_lookup(tce->env, dc->tag) == NULL) {
    // Define the type of the data constructor "function" in the local
    // term environment.
    tce_closure_t bdt;

    list_iterate(dc->params, valid_type_i, NULL);

    // This is a bit subtle: easier to build it in reverse.
    bdt.type = tce->type;
    list_iterate(list_reverse(dc->params), build_datacons_type_i, &bdt);
    list_reverse(dc->params);

//     fprintf(stdout, "%s :: ", dc->tag);
//     pp_type(stdout, bdt.type);
//     fprintf(stdout, "\n");

    env_add_binding(tce->env, dc->tag, bdt.type);
  } else {
    tc_error("verify_datacons_def_i: data constructor '%s' is not unique.\n", dc->tag);
  }

  return 1;
}

static int verify_user_type_i(void *data, void *info)
{
  env_t *env = (env_t *)info;
  type_t *t = (type_t *)data;
  tce_closure_t tce;
  type_t *uset;

  // Allocate a return type for the data constructor "functions" we build.
  uset = alloc_type(t_user_defined);
  user_def_use_name(uset) = user_def_datacons_name(t);

  tce.env = env;
  tce.type = uset;

  list_iterate(user_def_datacons_dcs(t), verify_datacons_def_i, &tce);

  return 1;
}

static void verify_user_types(env_t *env, list_t *types)
{
  list_iterate(types, verify_user_type_i, env);
}

//**************************************

int types_check(program_t *program)
{
  env_t *env;

  // Create the initial global type map.
  typeMap = initial_type_environment(program->types);

  // Check the user-defined types and:
  //  - define them in typeMap
  //  - define the data constructors in the environment
  ///
  env = builtin_bindings_types();
  verify_user_types(env, program->types);

  // Check each binding.
  tc_bindings(env, program->bindings);

  // Check the 'main' expression.
  tc_expr(env, program->main);

  return errors == 0;
}
