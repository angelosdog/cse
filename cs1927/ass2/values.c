//
// Values.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced August 2008.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "errors.h"
#include "eval.h"
#include "prettyprinter.h"
#include "util.h"
#include "values.h"

static void print_value_prefix(FILE *outf, value_t *v);
static void print_value_tree(FILE *outf, value_t *v);

// Print data constructors inline or as trees.
static int tree_printer = 0;

void toggle_tree_printer(void)
{
  tree_printer = !tree_printer;
}

// For the top-level value, choose to either print it in prefix form
// or as a tree.
void print_value(FILE *outf, value_t *v)
{
  if (tree_printer) {
    print_value_tree(outf, v);
  } else {
    print_value_prefix(outf, v);
  }
}

value_t *alloc_value(value_type_t type)
{
  value_t *new;

  if ((new = (value_t *)util_malloc(sizeof(value_t))) == NULL) {
    error("alloc_value: out of memory.\n");
  }

  new->type = type;

  return new;
}

// A little closure to print lists of values.
typedef struct {
  FILE *outf;
  int first;
} pv_closure_t;

static int print_value_list_i(void *data, void *info)
{
  pv_closure_t *c = (pv_closure_t *)info;
  value_t *v = (value_t *)data;

  if (c->first) {
    c->first = 0;
  } else {
    fprintf(c->outf, ", ");
  }

  print_value_prefix(c->outf, v);

  return 1;
}

static int print_values_i(void *data, void *info)
{
  FILE *outf = (FILE *)info;
  value_t *v = (value_t *)data;

  fprintf(outf, " ");
  print_value_prefix(outf, v);

  return 1;
}

int print_env_value_i(char *name, void *data, FILE *outf)
{
//   value_t *v = (value_t *)data;

  fprintf(outf, " %s", name);
//   fprintf(outf, "%s: ", name);
//   print_value_prefix(outf, v);
//   fprintf(outf, "\n");

  return 1;
}

static void print_datacons(FILE *outf, value_t *v)
{
  if (strcmp(datacons_tag(v), listEmptyTag) == 0) {
    // Empty list
    fprintf(outf, listEmptyTag);
  } else if (strcmp(datacons_tag(v), listConsTag) == 0) {
    // List constructor: assume lists are heterogenous.
    if (thunk_force(list_nth(datacons_params(v), 0))->type == v_char) {
      do {
        value_t *hv = thunk_force(list_head(datacons_params(v)));
        fprintf(outf, "%c", char_val(hv));
        // Next list element.
        v = thunk_force(list_nth(datacons_params(v), 1));
        // IMPROVE this could be better: while datacons, listcons, char...
      }	while (strcmp(datacons_tag(v), listEmptyTag) != 0);
    } else {
      int first = 1;

      fprintf(outf, "[");
      while (v->type == v_datacons
            && (strcmp(datacons_tag(v), listConsTag) == 0)) {
        if (first) {
          first = 0;
        } else {
          fprintf(outf, ", ");
        }
        print_value_prefix(outf, list_head(datacons_params(v)));
        v = thunk_force(list_nth(datacons_params(v), 1));
      }
      fprintf(outf, "]");
    }
  } else {
    fprintf(outf, "(%s", datacons_tag(v));
    list_iterate(datacons_params(v), print_values_i, outf);
    fprintf(outf, ")");
  }
}

static void print_value_prefix(FILE *outf, value_t *v)
{
  switch (v->type) {
  default:
  case v_unused:
    if (*(int *)NULL) {
      printf("should crash.\n");
    }
    break;
  case v_bool:
    fprintf(outf, "%s", bool_val(v) ? "True" : "False");
    break;
  case v_char:
    fprintf(outf, "'%c'", char_val(v));
    break;
  case v_closure:
    fprintf(outf, "closure: parameters:");
    list_iterate(closure_params(v), pp_var_i, outf);
    fprintf(outf, "\nclosure: body: ");
    pp_expr(outf, closure_body(v), 0);
    // IMPROVE: need to be cleverer here, environments could be recursively referenced.
//     fprintf(outf, "\nclosure: environment:\n");
//     env_print(outf, closure_env(v), print_env_value_i);
    fprintf(outf, "\n");
    break;
  case v_datacons:
    print_datacons(outf, v);
    break;
  case v_num:
    fprintf(outf, "%g", num_val(v));
    break;
  case v_tuple:
    {
      pv_closure_t c = {outf, 1};

      fprintf(outf, "(");
      list_iterate(tuple_val(v), print_value_list_i, &c);
      fprintf(outf, ")");
      break;
    }
  case v_thunk:
//     fprintf(outf, "<< thunk >> ");
//     fprintf(outf, "thunk: environment:");
//     env_print(outf, thunk_env(v), print_env_value_i);

    print_value_prefix(outf, thunk_force(v));
    break;
  case v_builtin_fn:
    fprintf(outf, "<< builtin function >>");
    break;
  }
}

// ****************************************

// Print data constructors as trees.

typedef struct {
  FILE *outf;
  char *prefix;
  int prefix_len;
  int single_child;
} dt_closure_t;

static int draw_subtrees(value_t *v, dt_closure_t *c);

static void draw_node(value_t *v, dt_closure_t *c)
{
  switch (v->type) {
  default:
    print_value_prefix(c->outf, v);
    fprintf(c->outf, "\n");
    break;

  case v_datacons:
    fprintf(c->outf, "%s\n", datacons_tag(v));
    {
      list_t *args = datacons_params(v);

      while (!list_isEmpty(args)) {
        value_t *p = list_head(args);

        args = list_new_tail(args); // FIXME leak.
        c->single_child = list_isEmpty(args);
        draw_subtrees(p, c);
      }
    }
    break;

  case v_thunk:
    draw_node(thunk_force(v), c);
    break;
  }
}

static int draw_subtrees(value_t *v, dt_closure_t *c)
{
  dt_closure_t c1;

  // We'd like to say "print out c->prefix_len characters" but printf doesn't support that.
  fprintf(c->outf, "%s|\n%s%c- ", c->prefix, c->prefix, c->single_child ? '`' : '+');

  c1 = *c;

  c1.prefix[c1.prefix_len++] = c->single_child ? ' ' : '|';
  c1.prefix[c1.prefix_len++] = ' ';
  c1.prefix[c1.prefix_len++] = ' ';
  c1.prefix[c1.prefix_len] = '\0';

  draw_node(v, &c1);

  c->prefix[c->prefix_len] = '\0';

  return 1;
}

static void print_value_tree(FILE *outf, value_t *v)
{
  switch (v->type) {
  default:
    print_value_prefix(outf, v);
    break;

  case v_datacons:
    {
      char prefix[100]; // FIXME should be 3 * tree depth + 1
      dt_closure_t c;

      prefix[0] = '\0';

      c.outf = outf;
      c.prefix = prefix;
      c.prefix_len = 0;

      draw_node(v, &c);
    }
    break;
  }
}
