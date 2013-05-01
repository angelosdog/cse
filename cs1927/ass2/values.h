//
// Values.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced August 2008.
//
// Re-use some types from parser.h to ease some C pain.
//

#ifndef _VALUES_H_
#define _VALUES_H_

#include "environment.h"
#include "parser.h"

typedef struct value value_t;

//
// A value is anything a variable could be bound to.
//
// For first-order strict, these are:
//
//  - ground types (Bool, Char, Int, List, Tuple, user-defined types)
//
// For higher-order strict add:
//
//  - functions (closures: function bodies (expressions) and environments)
//
// For laziness add:
//
//  - thunks (like closures, but at all types).
//
//
typedef enum {
  v_unused,		// don't use the 0 entry.
  v_bool,		// boolean.
  v_char,		// character.
  v_datacons,		// data constructor.
  v_num,		// integer.
  v_tuple,		// tuple.

  v_closure,		// Function closure, for higher-order functions.
  v_thunk,              // Thunk, for lazy evaluation.

  v_builtin_fn	        // Built-in function, implemented in C.
} value_type_t;

// A closure is a pair of the function to be evaluated and the
// environment in which to do it.
typedef struct closure {
  list_t *params; // List of parameter names (char *).
  expr_t *body;
  env_t *env;
} closure_t;

#define closure_params(val) ((val)->data.closure.params)
#define closure_body(val) ((val)->data.closure.body)
#define closure_env(val) ((val)->data.closure.env)

//
// You can ignore thunks if you're not dealing with laziness.
//
// A thunk starts out as a pair of the expression to be evaluated and
// the environment in which to do it. Once this evaluation happens we
// cache the result. Without caching we get call-by-name; with it, we
// get call-by-need (lazy evaluation). Ask Google for more info.
//
// We can record that evaluation has happened by setting the
// environment to NULL.
//
typedef struct thunk {
  env_t *env;
  union {
    expr_t *expr;
    value_t *val;
  } expr_val;
} thunk_t;

#define thunk_env(v) ((v)->data.thunk.env)
#define thunk_expr(v) ((v)->data.thunk.expr_val.expr)
#define thunk_val(v) ((v)->data.thunk.expr_val.val)

// A built-in function takes a list of (value_t *) arguments and
// produces a value. The function is invoked when it has the given
// number of arguments.
typedef struct builtin {
  int num_params;
  list_t *args; // Track arguments in a partial-application context.
  value_t *(*fn)(list_t *args);
} builtin_t;

#define builtin_num_params(v) ((v)->data.builtin.num_params)
#define builtin_args(v) ((v)->data.builtin.args)
#define builtin_fn(v) ((v)->data.builtin.fn)

// Use the macros in parser.h to access this structure.
struct value {
  value_type_t type;
  union {
    struct bconst   bconst;
    struct cconst   cconst;
    struct nconst   nconst;

    // These may contain values, recursively.
    struct datacons datacons;
    struct tuple    tuple;

    struct closure  closure;
    struct thunk    thunk;
    struct builtin  builtin;
  } data;
};

void toggle_tree_printer(void);

value_t *alloc_value(value_type_t type);

void print_value(FILE *outf, value_t *value);

int print_env_value_i(char *name, void *data, FILE *outf);

#endif // _VALUES_H_
