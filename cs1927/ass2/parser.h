//
// The parser.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01 (SimpleC).
// Redone for FLAN July 2008.
//

#ifndef _PARSER_H_
#define _PARSER_H_

//*********************************************************

// Forward-reference types needed in other headers.
typedef struct expr expr_t;

#include "list.h"

//*********************************************************
// Data types.

typedef struct type type_t;

// The types of types.
typedef enum {
  t_unused,
  t_bool,
  t_char,
  t_fn,
  t_num,
  t_list,
  t_tuple,
  t_user_defined,
  t_undef // Used for type-checking the empty list.
} type_type_t;

// Auxiliary structures.

struct t_fn {
  type_t *arg_type;
  type_t *return_type;
};

#define fn_arg_type(type) ((type)->data.fn.arg_type)
#define fn_ret_type(type) ((type)->data.fn.return_type)

struct t_list {
  type_t *list_elt_type;
};

#define list_elt_type(type) ((type)->data.list.list_elt_type)

struct t_tuple {
  list_t *types; // A list of types (type_t *).
};

#define tuple_types(type) ((type)->data.tuple.types)

// User-defined types, Algebraic Data Types, sums-of-products.

// The definition of a user-defined type.
typedef struct t_user_def t_user_def_t;
struct t_user_def {
  char *name;
  list_t *dcs;		// List of data constructors (datacons_t *).
};
// The use of a user-defined type (its name).
struct t_user_def_use {
  char *name;
};

#define user_def_datacons_name(type) ((type)->data.user_def.name)
#define user_def_datacons_dcs(type) ((type)->data.user_def.dcs)
#define user_def_use_name(type) ((type)->data.user_def_use.name)

// A type.

struct type {
  type_type_t type;
  union {
    struct t_fn           fn;
    struct t_list         list;
    struct t_tuple        tuple;
    struct t_user_def     user_def;
    struct t_user_def_use user_def_use;
  } data;
};

//*********************************************************
// Parse Tree node types.
typedef enum {
  p_unused,		// don't use the 0 entry.
  p_add,		// '+'
  p_and,		// '&&'
  p_bconst,		// boolean constant.
  p_cconst,		// character constant.
  p_datacons,		// data constructor.
  p_div,		// '/'
  p_eqop,		// '=='
  p_fncall,		// function call.
  p_ge,			// '>='
  p_gt,			// '>'
  p_ite,		// "if"/"then"/"else"
  p_le,			// '<='
  p_let,		// "let"
  p_listcons,		// list constructor ':'.
  p_listempty,		// empty list '[]'.
  p_listlit,		// list literal, e.g. [1, 2+3, 4].
  p_lt,			// '<'
  p_match,		// "match"
  p_mod,		// '%'
  p_mul,		// '*'
  p_nconst,		// numeric constant.
  p_ne,			// '!='
  p_negate,		// '!'
  p_or,			// '||'
  p_sub,		// '-'
  p_tuple,		// Tuple.
  p_var 		// Variable.
} expr_type_t;

// Bindings: a variable name, a type, a (possibly empty) list of
// parameters, and an expression.
typedef struct binding binding_t;
struct binding {
  int line_num;
  char *name;
  type_t *type;
  list_t *params; // List of variables (char *).
  expr_t *body;
};

// Variables.
struct variable {
  char *name;
};

#define var_name(expr) ((expr)->data.var.name)

// Data constructors, triple duty. 'params' is a list of:
//    - expressions (expr_t *), for data constructor expressions.
//    - variable names (char *) for patterns.
//    - types (type_t *) for user-defined types.
//
typedef struct datacons datacons_t;
struct datacons {
  char *tag;
  list_t *params; // List of types (type_t *).
};

#define datacons_tag(expr) ((expr)->data.datacons.tag)
#define datacons_params(expr) ((expr)->data.datacons.params)

// Constants.

// Boolean constants.
struct bconst {
    int val;
};

#define bool_val(expr) ((expr)->data.bconst.val)

// Character constants.
struct cconst {
    char val;
};

#define char_val(expr) ((expr)->data.cconst.val)

// Numeric constants.
struct nconst {
    double val;
};

#define num_val(expr) ((expr)->data.nconst.val)

// List literals: e.g. [1,2,3]
struct list {
  list_t *val; // List of (expr_t *).
};

#define list_val(expr) ((expr)->data.list.val)

// Tuples: e.g. (1,2,3)
struct tuple {
  list_t *val; // List of (expr_t *).
};

#define tuple_val(expr) ((expr)->data.tuple.val)

// Binary operators.
struct binary {
    expr_t *left;
    expr_t *right;
};

#define binary_left(expr) ((expr)->data.binary.left)
#define binary_right(expr) ((expr)->data.binary.right)

// Unary operators.
struct unary {
    expr_t *unary;
};

#define unary_expr(expr) ((expr)->data.unary.unary)

// Function calls.
struct fncall {
  expr_t *fn;
  list_t *args; // List element type: expr_t
};

#define fncall_fn(expr) ((expr)->data.fncall.fn)
#define fncall_args(expr) ((expr)->data.fncall.args)

// "if"/"then"/"else" expressions.
struct ite {
  expr_t *ite_cond;
  expr_t *ite_then;
  expr_t *ite_else;
};

#define ite_cond(expr) ((expr)->data.ite.ite_cond)
#define ite_then(expr) ((expr)->data.ite.ite_then)
#define ite_else(expr) ((expr)->data.ite.ite_else)

// "let" expressions.
struct let {
  list_t *bindings; // Element type: binding_t
  expr_t *body;
};

#define let_bindings(expr) ((expr)->data.let.bindings)
#define let_body(expr) ((expr)->data.let.body)

// "match" expressions.

// Patterns: recycle most things from expr_t.

struct listcons {
  char *hvar;
  char *tvar;
};

#define listcons_hvar(pat) ((pat)->data.listcons.hvar)
#define listcons_tvar(pat) ((pat)->data.listcons.tvar)

typedef struct pattern pattern_t;
struct pattern {
  expr_type_t type; // Slight hack, but eases pattern matching.
  int line_num;
  union {
    struct bconst   bconst;
    struct cconst   cconst;
    struct nconst   nconst;

    struct variable var;

    struct listcons listcons;

    // These may contain lists of variable names (char *)
    struct datacons datacons;
    struct tuple    tuple;
  } data;
};

typedef struct clause clause_t;
struct clause {
  pattern_t *pattern;
  expr_t *body;
};

struct match {
  expr_t *scrutinee;
  list_t *clauses; // List of clauses (clause_t *)
};

#define match_scrutinee(expr) ((expr)->data.match.scrutinee)
#define match_clauses(expr) ((expr)->data.match.clauses)

// An expression node.
struct expr {
  expr_type_t type;
  int line_num;
  union {
    struct bconst   bconst;
    struct cconst   cconst;
    struct nconst   nconst;

    struct variable var;

    struct datacons datacons;
    struct list     list;
    struct tuple    tuple;

    struct ite      ite;
    struct let      let;
    struct match    match;

    struct binary   binary;
    struct unary    unary;
    struct fncall   fncall;
  } data;
};

//*********************************************************

typedef struct {
  list_t *types;		// User-defined types: a list of (type_t *).
  list_t *bindings;		// Top-level bindings: a list of (binding_t *).
  expr_t *main;			// The expression bound to "main".
} program_t;

//*********************************************************
// Exported functions.

// Parse a list of user-defined types ("data = ...")
list_t *typedefs(void);

// Parse an entire program.
program_t *parse(void);

#endif // _PARSER_H_
