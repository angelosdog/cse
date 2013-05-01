//
// The evaluator (first-order strict).
//
// This should be quite straightfoward:
//
//  - disallow "let" bindings hence only require a global symbol table
//    for function definitions and local ones for function parameters.
//
// To keep things similar to the other evaluators, we use environments
// everywhere.
//
// Note this evaluator is just to show the baseline functionality I
// expect everyone can implement.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "errors.h"
#include "eval.h"
#include "list.h"
#include "parser.h"
#include "prettyprinter.h"
#include "values.h"

// The global environment, containting the top-level bindings.
static env_t *global_env;

//**************************************
// Recursively evaluate a program.

static value_t *e_expr(env_t *env, expr_t *expr);

// A little closure to evaluate lists of values.
typedef struct {
  env_t *env;
  list_t *list;
} eli_closure_t;

// Note the list is reversed, by construction.
static int e_expr_list_i(void *data, void *info)
{
  eli_closure_t *c = (eli_closure_t *)info;
  expr_t *expr = (expr_t *)data;

  list_cons(e_expr(c->env, expr), c->list);

  return 1;
}

// Use with list_zip_with: Add a list of variables and their
// corresponding values to an environment.
static int e_bind_params_i(void *data_l, void *data_r, void *info)
{
  env_t *env = (env_t *)info;
  char *var = (char *)data_l;
  value_t *val = (value_t *)data_r;

  env_add_binding(env, var, val);

  return 1;
}

//**************************************

// A little closure to match patterns.
typedef struct {
  value_t *val;
  env_t *env;
  expr_t *match_body;
} pm_closure_t;

// Mapped over the clauses in a "match" expression.
static int e_match_pat_i(void *data, void *info)
{
  pm_closure_t *pmc = (pm_closure_t *)info;
  clause_t *c = (clause_t *)data;

  // The style of pattern matching depends on the type of the pattern:
  //   - constants match literally
  //   - variables match anything (and extend the environment)
  //   - tuples always match (and extend the environment)
  //   - data constructors are more complex.
  ///

  switch (c->pattern->type) {
    // Constants: match literally, no binding.
  case p_bconst:
    if (bool_val(c->pattern) == bool_val(pmc->val)) {
      pmc->match_body = c->body;
    }
    break;
  case p_cconst:
    if (char_val(c->pattern) == char_val(pmc->val)) {
      pmc->match_body = c->body;
    }
    break;
  case p_nconst:
    if (num_val(c->pattern) == num_val(pmc->val)) {
      pmc->match_body = c->body;
    }
    break;
  case p_listempty:
    if (pmc->val->type == v_datacons && strcmp(datacons_tag(pmc->val), listEmptyTag) == 0) {
      pmc->match_body = c->body;
    }
    break;

  case p_var:
    // Matches anything. Bind it.
    env_add_binding(pmc->env, var_name(c->pattern), pmc->val);
    pmc->match_body = c->body;
    break;

  case p_listcons:
    // Check the list contains at least one element, then bind variables.
    if (pmc->val->type == v_datacons && strcmp(datacons_tag(pmc->val), listConsTag) == 0) {
      value_t *head;
      value_t *tail;

      head = list_nth(datacons_params(pmc->val), 0);
      tail = list_nth(datacons_params(pmc->val), 1);

      env_add_binding(pmc->env, listcons_hvar(c->pattern), head);
      env_add_binding(pmc->env, listcons_tvar(c->pattern), tail);

      pmc->match_body = c->body;
    }
    break;

  case p_datacons:
    // Check the tag matches, then bind the arguments (if any).
    if (strcmp(datacons_tag(c->pattern), datacons_tag(pmc->val)) == 0) {
      list_zip_with(datacons_params(c->pattern),
                    datacons_params(pmc->val),
                    e_bind_params_i, pmc->env);

      pmc->match_body = c->body;
    }
    break;

  case p_tuple:
    // Always matches (assuming the program type checks). Bind the variables.
    list_zip_with(tuple_val(c->pattern),
                  tuple_val(pmc->val),
                  e_bind_params_i, pmc->env);
    pmc->match_body = c->body;
    break;

  default:
    error("INTERNAL pattern match: invalid pattern.\n");
  }

  return pmc->match_body == NULL;
}

//**************************************
// Function call and data constructor evaluation.

// This is where most of the strict/lazy distinction is.
static value_t *e_fncall(env_t *env, expr_t *expr)
{
  eli_closure_t c;
  binding_t *fn;

  // Call-by-value (strict function calls): evaluate each argument to
  // a value in the given environment.
  c.env = env;
  c.list = list_empty();
  list_iterate(fncall_args(expr), e_expr_list_i, &c);
  list_reverse(c.list);

  switch (fncall_fn(expr)->type) {

  case p_var:
    // The function is literally the name of a function, and is
    // defined in the global environment.

    fn = (binding_t *)env_lookup(global_env, var_name(fncall_fn(expr)));
    assert(fn != NULL);

    // We must have exactly as many arguments as parameters.
    assert(list_length(c.list) == list_length(fn->params));

    // Bind the function's parameters to the given arguments in a new
    // scope derived from the global scope.
    env = global_env;
    env_new_scope(&env);
    list_zip_with(fn->params,
                  c.list,
                  e_bind_params_i, env);

    // Evaluate the function's body in the new environment.
    return e_expr(env, fn->body);

  case p_datacons:
    {
      value_t *result;

      result = alloc_value(v_datacons);
      datacons_tag(result) = datacons_tag(fncall_fn(expr));
      datacons_params(result) = c.list;

      // FIXME we'd like to assert that we got the right number of
      // arguments, but we don't know how many the data constructor
      // wanted.

      return result;
    }

    default:
      fprintf(stdout, "e_fncall: expression:\n");
      pp_expr(stdout, fncall_fn(expr), 2);
      fprintf(stdout, "\non line %d is not a function-variable or a data constructor.\n", fn->line_num);
      error("");
      return NULL;
  }
}

//**************************************

static int add_binding_i(void *data, void *info)
{
  binding_t *b = (binding_t *)data;

  if (b->type->type == t_fn) {
    // First order: Just add the bindings directly.
    env_add_binding(global_env, b->name, b);
  } else {    
    // Body is constant.
    env_add_binding(global_env, b->name, e_expr(global_env, b->body));
  }

  return 1;
}

static value_t *e_datacons(env_t *env, expr_t *expr)
{
  value_t *result;

  result = alloc_value(v_datacons);
  datacons_tag(result) = datacons_tag(expr);
  datacons_params(result) = list_empty();

  return result;
}

static value_t *e_listempty(void)
{
  value_t *result;

  result = alloc_value(v_datacons);
  datacons_tag(result) = listEmptyTag;
  datacons_params(result) = list_empty();

  return result;
}

static value_t *e_listcons(env_t *env, expr_t *expr)
{
  value_t *result;
  value_t *head;
  value_t *tail;

  head = e_expr(env, binary_left(expr));
  tail = e_expr(env, binary_right(expr));

  result = alloc_value(v_datacons);
  datacons_tag(result) = listConsTag;
  datacons_params(result) = list_cons(head, list_cons(tail, list_empty()));

  return result;
}

static value_t *e_listlit_rec(env_t *env, list_t *exprs, int i, int limit)
{
  value_t *result;

  if (i < limit) {
    value_t *list_elt = e_expr(env, list_nth(exprs, i));
    result = alloc_value(v_datacons);

    datacons_tag(result) = listConsTag;
    datacons_params(result) = list_cons(list_elt,
                			list_cons(e_listlit_rec(env, exprs, i + 1, limit),
                				  list_empty()));
  } else {
    result = alloc_value(v_datacons);

    datacons_tag(result) = listEmptyTag;
    datacons_params(result) = list_empty();
  }

  return result;
}

static value_t *e_listlit(env_t *env, expr_t *expr)
{
  return e_listlit_rec(env, list_val(expr), 0, list_length(list_val(expr)));
}

static value_t *e_tuple(env_t *env, expr_t *expr)
{
  value_t *result;
  eli_closure_t c;

  c.env = env;
  c.list = list_empty();
  list_iterate(tuple_val(expr), e_expr_list_i, &c);
  list_reverse(c.list);

  result = alloc_value(v_tuple);
  tuple_val(result) = c.list;

  return result;
}

static value_t *e_ite(env_t *env, expr_t *expr)
{
  value_t *cond;

  cond = e_expr(env, ite_cond(expr));
  return bool_val(cond)
    ? e_expr(env, ite_then(expr))
    : e_expr(env, ite_else(expr));
}

static value_t *e_match(env_t *env, expr_t *expr)
{
  pm_closure_t pmc;

  // Evaluate the scrutinised expression.
  pmc.val = e_expr(env, match_scrutinee(expr));
  pmc.env = env;
  pmc.match_body = NULL;

  // Create a new scope for any variable bindings.
  env_new_scope(&pmc.env);
  list_iterate(match_clauses(expr), e_match_pat_i, &pmc);
  if (pmc.match_body == NULL) {
    fprintf(stdout, "e_match: the scrutinee in the 'match' expression:");
    pp_expr(stdout, expr, 2);
    fprintf(stdout, "\non line %d reduced to value:\n  ", expr->line_num);
    print_value(stdout, pmc.val);
    fprintf(stdout, "\nand failed to match any patterns.\n");
    error("");
    return NULL; // gcc dataflow
  } else {
    return e_expr(pmc.env, pmc.match_body);
  }    
}

static value_t *e_binary_op(env_t *env, expr_t *expr)
{
  value_t *result;
  value_t *l;
  value_t *r;

  l = e_expr(env, binary_left(expr));
  r = e_expr(env, binary_right(expr));
  result = alloc_value(v_unused);

  switch (expr->type) {
  case p_add:
    result->type = v_num;
    num_val(result) = num_val(l) + num_val(r);
    break;
  case p_and:
    result->type = v_bool;
    bool_val(result) = bool_val(l) && bool_val(r);
    break;
  case p_div:
    result->type = v_num;
    num_val(result) = num_val(l) / num_val(r);
    break;
  case p_ge:
    result->type = v_bool;
    bool_val(result) = num_val(l) >= num_val(r);
    break;
  case p_gt:
    result->type = v_bool;
    bool_val(result) = num_val(l) > num_val(r);
    break;
  case p_le:
    result->type = v_bool;
    bool_val(result) = num_val(l) <= num_val(r);
    break;
  case p_lt:
    result->type = v_bool;
    bool_val(result) = num_val(l) < num_val(r);
    break;
  case p_mod:
    result->type = v_num;
    num_val(result) = (long)num_val(l) % (long)num_val(r);
    break;
  case p_mul:
    result->type = v_num;
    num_val(result) = num_val(l) * num_val(r);
    break;
  case p_or:
    result->type = v_bool;
    bool_val(result) = bool_val(l) || bool_val(r);
    break;
  case p_sub:
    result->type = v_num;
    num_val(result) = num_val(l) - num_val(r);
    break;

  default:
    error("Not a binary operator.");
    return NULL; // gcc dataflow
  }

  return result;
}

// Equality checking is slightly tricky (in full generality).

static int equality_test(value_t *l, value_t *r);

static int equality_test_i(void *data_l, void *data_r, void *info)
{
  int *equal = (int *)info;
  value_t *l = (value_t *)data_l;
  value_t *r = (value_t *)data_r;

  if (*equal) {
    *equal = equality_test(l, r);
  }

  return 1;
}

static int equality_test(value_t *l, value_t *r)
{
  int result;

  switch (l->type) {
  default:
  case v_unused:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    break;

  case v_bool:
    result = bool_val(l) == bool_val(r);
    break;
  case v_char:
    result = char_val(l) == char_val(r);
    break;
  case v_datacons:
    if (strcmp(datacons_tag(l), datacons_tag(r)) == 0) {
      result = 1;

      list_zip_with(datacons_params(l),
                    datacons_params(r),
                    equality_test_i, &result);
    } else {
      result = 0;
    }
    break;
  case v_num:
    result = num_val(l) == num_val(r);
    break;
  case v_tuple:
    result = 1;

    list_zip_with(tuple_val(l),
                  tuple_val(r),
                  equality_test_i, &result);

    break;
  }

  return result;
}

static value_t *e_equals(env_t *env, expr_t *l, expr_t *r)
{
  value_t *result;

  result = alloc_value(v_bool);
  bool_val(result) = equality_test(e_expr(env, l), e_expr(env, r));

  return result;
}

static value_t *e_expr(env_t *env, expr_t *expr)
{
  value_t *result;

  switch (expr->type) {
  default: // This is to handle invalid tags.
  case p_unused:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    return NULL;

  case p_add:
  case p_and:
  case p_div:
  case p_ge:
  case p_gt:
  case p_le:
  case p_lt:
  case p_mod:
  case p_mul:
  case p_or:
  case p_sub:
    result = e_binary_op(env, expr);
    break;

  case p_bconst:
    result = alloc_value(v_bool);
    bool_val(result) = bool_val(expr);
    break;
  case p_cconst:
    result = alloc_value(v_char);
    char_val(result) = char_val(expr);
    break;
  case p_datacons:
    result = e_datacons(env, expr);
    break;
  case p_eqop:
    result = e_equals(env, binary_left(expr), binary_right(expr));
    break;
  case p_fncall:
    result = e_fncall(env, expr);
    break;
  case p_nconst:
    result = alloc_value(v_num);
    num_val(result) = num_val(expr);
    break;
  case p_ite:
    result = e_ite(env, expr);
    break;
  case p_let:
    error("eval-fos: 'let' not implemented.\n");
    return NULL;
  case p_listcons:
    result = e_listcons(env, expr);
    break;
  case p_listlit:
    result = e_listlit(env, expr);
    break;
  case p_listempty:
    result = e_listempty();
    break;
  case p_match:
    result = e_match(env, expr);
    break;
  case p_ne:
    result = e_equals(env, binary_left(expr), binary_right(expr));
    bool_val(result) = !bool_val(result);
    break;
  case p_negate:
    result = e_expr(env, unary_expr(expr));
    bool_val(result) = !bool_val(result);
    break;
  case p_tuple:
    result = e_tuple(env, expr);
    break;
  case p_var:
    result = env_lookup(env, var_name(expr));
    if (result == NULL) {
      error("e_expr: variable '%s' not in scope on line %d.\n", var_name(expr), expr->line_num);
    }
    break;
  }

  return result;
}

//**************************************
// Top-level.

value_t *evaluate_program_fos(env_t *env, program_t *program)
{
  global_env = env;
  list_iterate(program->bindings, add_binding_i, NULL);

  // Evaluate the 'main' expression in the global environment.
  return e_expr(global_env, program->main);
}
