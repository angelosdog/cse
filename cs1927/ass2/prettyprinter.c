//
// The Pretty Printer.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 22/08/01 (SimpleC).
// Redone for FLAN July 2008.
//
//

#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "prettyprinter.h"

static void indent(FILE *outf, int level)
{
  char s[level + 1];
  int i;

  for (i = 0; i < level; i++) {
    s[i] = ' ';
  }
  s[level] = '\0';

  fprintf(outf, s);
}

// Some iterators need more context. Provide a generic closure for them.
typedef struct {
  FILE *outf;
  int level;
  int first;
} closure_t;

static void pp_bindings(FILE *outf, list_t *bs, int level);

int pp_var_i(void *data, void *info)
{
  FILE *outf = (FILE *)info;
  char *var = (char *)data;

  fprintf(outf, " %s", var);

  return 1;
}

static int pp_var_list_i(void *data, void *info)
{
  closure_t *c = (closure_t *)info;
  char *var = (char *)data;

  if (c->first) {
    c->first = 0;
  } else {
    fprintf(c->outf, ", ");
  }

  fprintf (c->outf, "%s", var);

  return 1;
}

static int pp_expr_list_i(void *data, void *info)
{
  closure_t *c = (closure_t *)info;
  expr_t *expr = (expr_t *)data;

  if (c->first) {
    c->first = 0;
  } else {
    fprintf(c->outf, ", ");
  }

  pp_expr(c->outf, expr, c->level); // FIXME level

  return 1;
}

static int pp_expr_i(void *data, void *info)
{
  closure_t *c = (closure_t *)info;
  expr_t *expr = (expr_t *)data;

  fprintf(c->outf, " ");
  pp_expr(c->outf, expr, c->level + 1);

  return 1;
}

static int pp_type_list_i(void *data, void *info)
{
  closure_t *c = (closure_t *)info;
  type_t *type = (type_t *)data;

  if (c->first) {
    c->first = 0;
  } else {
    fprintf(c->outf, ", ");
  }

  pp_type(c->outf, type);

  return 1;
}

static void pp_types(FILE *outf, list_t *types)
{
  closure_t c = {outf, 0, 1};

  list_iterate(types, pp_type_list_i, &c);
}

static void pp_type_fn(FILE *outf, type_t *type)
{
  fprintf(outf, "(");
  pp_type(outf, fn_arg_type(type));
  fprintf(outf, " -> ");
  pp_type(outf, fn_ret_type(type));
  fprintf(outf, ")");
}

static void pp_type_list(FILE *outf, type_t *type)
{
  fprintf(outf, "[");
  pp_type(outf, list_elt_type(type));
  fprintf(outf, "]");
}

static void pp_type_tuple(FILE *outf, type_t *type)
{
  fprintf(outf, "(");
  pp_types(outf, tuple_types(type));
  fprintf(outf, ")");
}

static void pp_type_user_defined(FILE *outf, type_t *type)
{
  fprintf(outf, user_def_use_name(type));
}

int pp_type_i(void *data, void *info)
{
  type_t *t = (type_t *)data;
  FILE *outf = (FILE *)info;

  fprintf(outf, " ");
  pp_type(outf, t);

  return 1;
}

void pp_type(FILE *outf, type_t *type)
{
  switch (type->type) {
  case t_bool:
    fprintf(outf, "Bool");
    break;
  case t_char:
    fprintf(outf, "Char");
    break;
  case t_fn:
    pp_type_fn(outf, type);
    break;
  case t_num:
    fprintf(outf, "Num");
    break;
  case t_list:
    pp_type_list(outf, type);
    break;
  case t_tuple:
    pp_type_tuple(outf, type);
    break;
  case t_user_defined:
    pp_type_user_defined(outf, type);
    break;

    // HACK: if an identifier is not in scope, we get a 't_undef'.
  case t_undef:
    fprintf(outf, "<< unknown >>");
    break;

  default:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    break;
  }
}

static int pp_datacons_i(void *data, void *info)
{
  closure_t *c = (closure_t *)info;
  datacons_t *dc = (datacons_t *)data;

  if (c->first) {
    c->first = 0;
  } else {
    fprintf(c->outf, "   | ");
  }
  fprintf(c->outf, dc->tag);
  list_iterate(dc->params, pp_type_i, c->outf);
  fprintf(c->outf, "\n");

  return 1;
}

int datacons_compare(void *l, void *r)
{
  datacons_t *dcl = (datacons_t *)l;
  datacons_t *dcr = (datacons_t *)r;

  return strcmp(dcl->tag, dcr->tag);
}

// Print out a user-defined type definition.
static int pp_user_defined_type_i(void *data, void *info)
{
  FILE *outf = (FILE *)info;
  type_t *t = (type_t *)data;
  closure_t c;

  fprintf(outf, "data %s\n   = ", user_def_datacons_name(t));
  c.outf = outf;
  c.first = 1;
  list_iterate(list_sort(user_def_datacons_dcs(t), datacons_compare), pp_datacons_i, &c);
  fprintf(outf, "\n");

  return 1;
}

static void pp_tuple(FILE *outf, expr_t *expr, int level)
{
  closure_t c = {outf, level, 1};

  fprintf(outf, "(");
  list_iterate(tuple_val(expr), pp_expr_list_i, &c);
  fprintf(outf, ")");
}

static void pp_binaryop(FILE *outf, expr_t *expr, int level, char *symbol)
{
  fprintf(outf, "(");
  pp_expr(outf, binary_left(expr), level);
  fprintf(outf, symbol);
  pp_expr(outf, binary_right(expr), level);
  fprintf(outf, ")");
}

static void pp_unaryop(FILE *outf, expr_t *expr, int level, char *symbol)
{
  fprintf(outf, symbol);
  fprintf(outf, "(");
  pp_expr(outf, unary_expr(expr), level);
  fprintf(outf, ")");
}

static void pp_fncall(FILE *outf, expr_t *expr, int level)
{
  closure_t c = {outf, level, 1};

  fprintf(outf, "(");
  pp_expr(outf, fncall_fn(expr), level);
  list_iterate(fncall_args(expr), pp_expr_i, &c);
  fprintf(outf, ")");
}

static void pp_let(FILE *outf, expr_t *expr, int level)
{
  fprintf(outf, "\n");
  indent(outf, level);
  fprintf(outf, "(let\n");
  pp_bindings(outf, let_bindings(expr), level + 2);
  indent(outf, level);
  fprintf(outf, "in ");
  pp_expr(outf, let_body(expr), level + 2);
  fprintf(outf, ")");
}

static void pp_pat_datacons(FILE *outf, pattern_t *pat)
{
  fprintf(outf, datacons_tag(pat));
  list_iterate(datacons_params(pat), pp_var_i, outf);
}

static void pp_pat_tuple(FILE *outf, pattern_t *pat)
{
  closure_t c = {outf, 0, 1};

  fprintf(outf, "(");
  list_iterate(tuple_val(pat), pp_var_list_i, &c);
  fprintf(outf, ")");
}

static void pp_ite(FILE *outf, expr_t *expr, int level)
{
  fprintf(outf, "\n");
  indent(outf, level);
  fprintf(outf, "(if ");
  pp_expr(outf, ite_cond(expr), level); // FIXME level
  fprintf(outf, " then ");
  pp_expr(outf, ite_then(expr), level + 2);
  fprintf(outf, " else ");
  pp_expr(outf, ite_else(expr), level + 2);
  fprintf(outf, "\n");
  indent(outf, level);
  fprintf(outf, "end)");
}

void pp_pattern(FILE *outf, pattern_t *pat)
{
  switch (pat->type) {
  case p_bconst:
    fprintf(outf, "%s", bool_val(pat) ? "True" : "False");
    break;
  case p_cconst:
    fprintf(outf, "'%c'", char_val(pat));
    break;
  case p_datacons:
    pp_pat_datacons(outf, pat);
    break;
  case p_nconst:
    fprintf(outf, "%g", num_val(pat));
    break;
  case p_listempty:
    fprintf(outf, "[]");
    break;
  case p_listcons:
    fprintf(outf, "%s : %s", listcons_hvar(pat), listcons_tvar(pat));
    break;
  case p_tuple:
    pp_pat_tuple(outf, pat);
    break;
  case p_var:
    fprintf(outf, "%s", var_name(pat));
    break;
  default:
    error("INTERNAL pp_pattern: not a pattern.\n");
  }
}

static int pp_clause_i(void *data, void *info)
{
  closure_t *c = (closure_t *)info;
  clause_t *clause = (clause_t *)data;

  fprintf(c->outf, "\n");
  if (c->first) {
    c->first = 0;
    indent(c->outf, c->level + 2); // FIXME level
  } else {
    indent(c->outf, c->level);
    fprintf(c->outf, "| ");
  }

  pp_pattern(c->outf, clause->pattern);
  fprintf(c->outf, " -> ");
  pp_expr(c->outf, clause->body, c->level + 4); // FIXME level

  return 1;
}

static void pp_clauses(FILE *outf, list_t *clauses, int level)
{
  closure_t c = {outf, level, 1};

  list_iterate(clauses, pp_clause_i, &c);
}

static void pp_match(FILE *outf, expr_t *expr, int level)
{
  fprintf(outf, "\n");
  indent(outf, level);
  fprintf(outf, "(match ");
  pp_expr(outf, match_scrutinee(expr), level); // FIXME level
  fprintf(outf, " with");
  pp_clauses(outf, match_clauses(expr), level + 2);
  fprintf(outf, "\n");
  indent(outf, level);
  fprintf(outf, "end)");
}

static void pp_listlit(FILE *outf, expr_t *expr, int level)
{
  closure_t c = {outf, level, 1};
  list_t *elts = list_val(expr);

  fprintf(outf, "[");
  list_iterate(elts, pp_expr_list_i, &c);
  fprintf(outf, "]");
}

void pp_expr(FILE *outf, expr_t *expr, int level)
{
  switch (expr->type) {
  case p_unused:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    break;
  case p_add:
    pp_binaryop(outf, expr, level, "+");
    break;
  case p_and:
    pp_binaryop(outf, expr, level, "&&");
    break;
  case p_bconst:
    fprintf(outf, "%s", bool_val(expr) ? "True" : "False");
    break;
  case p_cconst:
    fprintf(outf, "'%c'", char_val(expr));
    break;
  case p_datacons:
    fprintf(outf, datacons_tag(expr));
    break;
  case p_div:
    pp_binaryop(outf, expr, level, "/");
    break;
  case p_eqop:
    pp_binaryop(outf, expr, level, "==");
    break;
  case p_fncall:
    pp_fncall(outf, expr, level);
    break;
  case p_ge:
    pp_binaryop(outf, expr, level, ">=");
    break;
  case p_gt:
    pp_binaryop(outf, expr, level, ">");
    break;
  case p_nconst:
    fprintf(outf, "%g", num_val(expr));
    break;
  case p_ite:
    pp_ite(outf, expr, level);
    break;
  case p_le:
    pp_binaryop(outf, expr, level, "<=");
    break;
  case p_let:
    pp_let(outf, expr, level);
    break;
  case p_listcons:
    pp_binaryop(outf, expr, level, ":");
    break;
  case p_listlit:
    pp_listlit(outf, expr, level);
    break;
  case p_listempty:
    fprintf(outf, "[]");
    break;
  case p_lt:
    pp_binaryop(outf, expr, level, "<");
    break;
  case p_match:
    pp_match(outf, expr, level);
    break;
  case p_mod:
    pp_binaryop(outf, expr, level, "%%");
    break;
  case p_mul:
    pp_binaryop(outf, expr, level, "*");
    break;
  case p_ne:
    pp_binaryop(outf, expr, level, "!=");
    break;
  case p_negate:
    pp_unaryop(outf, expr, level, "!");
    break;
  case p_or:
    pp_binaryop(outf, expr, level, "||");
    break;
  case p_sub:
    pp_binaryop(outf, expr, level, "-");
    break;
  case p_tuple:
    pp_tuple(outf, expr, level);
    break;
  case p_var:
    fprintf(outf, "%s", var_name(expr));
    break;
  }
}

static void pp_binding(FILE *outf, binding_t *b, int level)
{
  indent(outf, level);
  fprintf(outf, "%s :: ", b->name);
  pp_type(outf, b->type);
  fprintf(outf, "\n");
  indent(outf, level);

  fprintf(outf, "%s", b->name);
  list_iterate(b->params, pp_var_i, outf);
  fprintf(outf, " = ");
  pp_expr(outf, b->body, level + 2);
  
  fprintf(outf, "\n");
  indent(outf, level);
  fprintf(outf, ";\n");
}

static int pp_binding_i(void *data, void *info)
{
  closure_t *c = (closure_t *)info;
  binding_t *b = (binding_t *)data;

  pp_binding(c->outf, b, c->level);

  return 1;
}

static void pp_bindings(FILE *outf, list_t *bs, int level)
{
  closure_t c = {outf, level, 1};

  list_iterate(bs, pp_binding_i, &c);
}

static void pp_main(FILE *outf, expr_t *e)
{
  fprintf(outf, "main = ");
  pp_expr(outf, e, 2);
  fprintf(outf, "\n");
}

static int binding_compare(void *l, void *r)
{
  binding_t *bl = (binding_t *)l;
  binding_t *br = (binding_t *)r;

  return strcmp(bl->name, br->name);
}

static int types_compare(void *l, void *r)
{
  type_t *tl = (type_t *)l;
  type_t *tr = (type_t *)r;

  return strcmp(user_def_datacons_name(tl), user_def_datacons_name(tr));
}

void pp_program(FILE *outf, program_t *program)
{
  list_iterate(list_sort(program->types, types_compare), pp_user_defined_type_i, outf);
  pp_bindings(outf, list_sort(program->bindings, binding_compare), 0);
  pp_main(outf, program->main);
}
