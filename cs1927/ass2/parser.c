//
// The parser.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01 (SimpleC).
// Redone for FLAN July 2008.
//
// Grammar can be found in "grammar".
//
// Conventions:
//  - the lexer is always setup with the next symbol to be consumed.
//  - right (tail) recursion is factored into while loops.
//  - parse functions can fail (i.e. return NULL).
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "errors.h"
#include "lexer.h"
#include "parser.h"
#include "types.h"
#include "util.h"

//*********************************************************
// Support functions.

//
// Allocates a new parse tree node.
//
//  - sets the type.
//  - sets the line number.
//
static expr_t *alloc_expr_node(expr_type_t type)
{
  expr_t *new;

  if ((new = (expr_t *)util_malloc(sizeof(expr_t))) == NULL) {
    error("alloc_expr_node: out of memory.\n");
  }

  new->type = type;
  new->line_num = line_num;

  return new;
}

//
// Allocates a new pattern node.
//
//  - sets the type.
//  - sets the line number.
//
static pattern_t *alloc_pat(expr_type_t type)
{
  pattern_t *new;

  if ((new = (pattern_t *)util_malloc(sizeof(pattern_t))) == NULL) {
    error("alloc_pat: out of memory.\n");
  }

  new->type = type;
  new->line_num = line_num;

  return new;
}

static binding_t *alloc_binding(char *name)
{
  binding_t *new;

  if ((new = (binding_t *)util_malloc(sizeof(binding_t))) == NULL) {
    error("alloc_binding: out of memory.\n");
  }

  new->line_num = line_num;
  new->name = name;

  return new;
}

static clause_t *alloc_clause(pattern_t *pattern, expr_t *body)
{
  clause_t *new;

  if ((new = (clause_t *)util_malloc(sizeof(clause_t))) == NULL) {
    error("alloc_clause: out of memory.\n");
  }

  new->pattern = pattern;
  new->body = body;

  return new;
}

static datacons_t *alloc_datacons(char *tag)
{
  datacons_t *new;

  if ((new = (datacons_t *)util_malloc(sizeof(datacons_t))) == NULL) {
    error("alloc_datacons: out of memory.\n");
  }

  new->tag = tag;

  return new;
}

//*********************************************************

static list_t *bindings(void);
static expr_t *expression(void);
static list_t *expressionList(void);
static type_t *type(int emptyOK);

// Constant         = "[]"
//  		 | "True" | "False"
// 		 | "'" Char "'"
// 		 | Num
// 		 | String
// AtomicExpr0  =
//   CONST:       Constant
//   LIST:      | "[" ExpressionList "]"
static expr_t *atomicExpr0(void)
{
  expr_t *new;

  switch (yylval.lexeme) {
  case EMPTYLIST:
    new = alloc_expr_node(p_listempty);
    lex();
    break;

  case LSBRACKET:
    {
      list_t *exprs;

      lex();
      exprs = expressionList();
      lex_expect(RSBRACKET, "atomicExpr0: expected ']'.\n");
      new = alloc_expr_node(p_listlit);
      list_val(new) = exprs;

      break;
    }

  case FALSE:
    new = alloc_expr_node(p_bconst);
    bool_val(new) = 0;
    lex();
    break;

  case TRUE:
    new = alloc_expr_node(p_bconst);
    bool_val(new) = 1;
    lex();
    break;

  case CHARCONST:
    new = alloc_expr_node(p_cconst);
    char_val(new) = yylval.charval;
    lex();
    break;

  case NUMCONST:
    new = alloc_expr_node(p_nconst);
    num_val(new) = yylval.numval;
    lex();
    break;

  default:
    new = NULL;
    break;
  }

  return new;
}

// AtomicExpr1  =
//   DATACONS:  | Identifier
//   TUPLES:    | "(" ExpressionList ")" -- If one expression, equivalent to that expression.
//   VAR:       | Variable
static expr_t *atomicExpr1(void)
{
  expr_t *new;

  switch (yylval.lexeme) {
  case IDENTIFIER:
    new = alloc_expr_node(p_datacons);
    datacons_tag(new) = strdup(yylval.strval);
    datacons_params(new) = list_empty();
    lex();
    break;

  case LPAREN:
    {
      list_t *exprs;

      lex();
      exprs = expressionList();
      lex_expect(RPAREN, "atomicExpr1: expected ')'.\n");
      if (list_length(exprs) == 1) {
        new = (expr_t *)list_head(exprs);
        list_destroy(&exprs);
      } else {
        new = alloc_expr_node(p_tuple);
        tuple_val(new) = exprs;
      }

      break;
    }

  case VARIABLE:
    new = alloc_expr_node(p_var);
    var_name(new) = strdup(yylval.strval);
    lex();
    break;

  default:
    new = NULL;
    break;
  }

  return new;
}

// SimpleExpr   = AtomicExpr0 | AtomicExpr1 Arguments
// Arguments    = AtomicExpr Arguments | EMPTY
// AtomicExpr   = AtomicExpr0 | AtomicExpr1
static expr_t *simpleExpr(void)
{
    expr_t *new;

    if ((new = atomicExpr0()) == NULL) {
      if ((new = atomicExpr1()) == NULL) {
        error("simpleExpr: parse error on line %d.\n", line_num);
      } else {
        expr_t *arg;
        list_t *args;

        args = list_empty();

        while ((arg = atomicExpr0()) || (arg = atomicExpr1())) {
          list_cons(arg, args);
        }

        if (list_length(args) == 0) {
          list_destroy(&args);
        } else {
          expr_t *fncall = alloc_expr_node(p_fncall);

          // The line number for this function call is where the function bit is.
          fncall->line_num = new->line_num;
          fncall_fn(fncall) = new;
          fncall_args(fncall) = list_reverse(args);

          new = fncall;
        }
      }
    }

    return new;
}

// UnaryExpr    = SimpleExpr | "!" SimpleExpr
static expr_t *unaryExpr(void)
{
  expr_t *new;

  if (yylval.lexeme == NEGOP) {
    lex();
    new = alloc_expr_node(p_negate);
    unary_expr(new) = simpleExpr();
  } else {
    new = simpleExpr();
  }

  return new;
}

// MulExpr      = UnaryExpr MulExpr'
// MulExpr'     = MulOp UnaryExpr MulExpr' | EMPTY
// MulOp        = "*" | "/" | "%"
static expr_t *mulExpr(void)
{
  expr_t *left = unaryExpr();

  while (1) {
    expr_type_t type;
    expr_t *right;
    expr_t *new;

    switch (yylval.lexeme) {
    case DIVIDE:
      type = p_div;
      break;
    case MODULUS:
      type = p_mod;
      break;
    case MULT:
      type = p_mul;
      break;
    default:
      return left;
    }

    lex();
    right = unaryExpr();

    new = alloc_expr_node(type);
    binary_left(new) = left;
    binary_right(new) = right;
    left = new;
  }
}

// AddExpr      = MulExpr AddExpr'
// AddExpr'     = AddOp MulExpr AddExpr' | EMPTY
// AddOp        = "+" | "-"
static expr_t *addExpr(void)
{
  expr_t *left = mulExpr();

  while (yylval.lexeme == PLUS || yylval.lexeme == MINUS) {
    expr_t *right;
    expr_t *new;
    expr_type_t type = (yylval.lexeme == PLUS) ? p_add : p_sub;

    lex();
    right = mulExpr();

    new = alloc_expr_node(type);
    binary_left(new) = left;
    binary_right(new) = right;
    left = new;
  }

  return left;
}

// RelExpr      = AddExpr RelExpr'
// RelExpr'     = RelOp AddExpr RelExpr' | EMPTY
// RelOp        = "<" | "<=" | ">" | ">="
static expr_t *relExpr(void)
{
  expr_t *left = addExpr();

  while (1) {
    expr_t *right;
    expr_t *new;
    expr_type_t type;

    switch (yylval.lexeme) {
    case LTOP:
      type = p_lt;
      break;
    case LEOP:
      type = p_le;
      break;
    case GTOP:
      type = p_gt;
      break;
    case GEOP:
      type = p_ge;
      break;
    default:
      return left;
    }

    lex();
    right = addExpr();

    new = alloc_expr_node(type);
    binary_left(new) = left;
    binary_right(new) = right;
    left = new;
  }
}

// EqExpr       = RelExpr EqExpr'
// EqExpr'      = EqOp RelExpr EqExpr' | EMPTY
// EqOp         = "==" | "!="
static expr_t *eqExpr(void)
{
  expr_t *left = relExpr();

  while (yylval.lexeme == EQOP || yylval.lexeme == NEOP) {
    expr_t *right;
    expr_t *new;
    expr_type_t type = (yylval.lexeme == EQOP) ? p_eqop : p_ne;

    lex();
    right = relExpr();

    new = alloc_expr_node(type);
    binary_left(new) = left;
    binary_right(new) = right;
    left = new;
  }

  return left;
}

// AndExpr      = EqExpr AndExpr'
// AndExpr'     = "&&" EqExpr AndExpr' | EMPTY
static expr_t *andExpr(void)
{
  expr_t *left = eqExpr();

  while (yylval.lexeme == ANDOP) {
    expr_t *right;
    expr_t *new;

    lex();
    right = eqExpr();

    new = alloc_expr_node(p_and);
    binary_left(new) = left;
    binary_right(new) = right;
    left = new;
  }

  return left;
}

// OrExpr       = AndExpr OrExpr'
// OrExpr'      = "||" AndExpr OrExpr' | EMPTY
static expr_t *orExpr(void)
{
  expr_t *left = andExpr();

  while (yylval.lexeme == OROP) {
    expr_t *right;
    expr_t *new;

    lex();
    right = andExpr();

    new = alloc_expr_node(p_or);
    binary_left(new) = left;
    binary_right(new) = right;
    left = new;
  }

  return left;
}

// ConsExpr     = OrExpr ConsExpr'
// ConsExpr'    = ":" ConsExpr | EMPTY
static expr_t *consExpr(void)
{
  expr_t *left = orExpr();

  if (yylval.lexeme == COLON) {
    expr_t *right;
    expr_t *new;

    lex();
    right = consExpr();

    new = alloc_expr_node(p_listcons);
    binary_left(new) = left;
    binary_right(new) = right;
    left = new;
  }

  return left;
}

static pattern_t *pattern_constant(void)
{
  pattern_t *pat;

  switch (yylval.lexeme) {
  case EMPTYLIST:
    pat = alloc_pat(p_listempty);
    lex();
    break;

  case FALSE:
    pat = alloc_pat(p_bconst);
    bool_val(pat) = 0;
    lex();
    break;

  case TRUE:
    pat = alloc_pat(p_bconst);
    bool_val(pat) = 1;
    lex();
    break;

  case CHARCONST:
    pat = alloc_pat(p_cconst);
    char_val(pat) = yylval.charval;
    lex();
    break;

  case NUMCONST:
    pat = alloc_pat(p_nconst);
    num_val(pat) = yylval.numval;
    lex();
    break;

  default:
    pat = NULL;
    break;
  }

  return pat;
}

char *variable(void)
{
  if (yylval.lexeme == VARIABLE) {
    char *var = strdup(yylval.strval);

    lex();
    return var;
  } else {
    error("variable: expected VARIABLE on line %d.\n", line_num);
    return NULL; // gcc dataflow.
  }
}

// VariableList     = Variable VariableList'
// VariableList'    = "," Variable VariableList' | EMPTY
static list_t *variableList(void)
{
  list_t *vars;

  vars = list_empty();
  list_cons(variable(), vars);

  while (yylval.lexeme == COMMA) {
    lex();
    list_cons(variable(), vars);
  }

  return list_reverse(vars);
}

// Pattern         = Constant | Identifier Variables | "(" VariableList ")" | Variable ConsPattern
// ConsOrList      = ":" Variable | EMPTY
// VariableList    = Variable VariableList'
// VariableList'   = "," Variable VariableList' | EMPTY
static pattern_t *pattern(void)
{
  pattern_t *pat;

  if ((pat = pattern_constant()) == NULL) {
    switch (yylval.lexeme) {
    case IDENTIFIER:
      {
        list_t *args;

        pat = alloc_pat(p_datacons);
        datacons_tag(pat) = strdup(yylval.strval);
        lex();

        args = list_empty();
        while (yylval.lexeme == VARIABLE) {
          list_cons(strdup(yylval.strval), args);
          lex();
        }

        datacons_params(pat) = list_reverse(args);
        break;
      }

    case LPAREN:
      {
        list_t *vars;

        lex(); // Consume ')'
        vars = variableList();
        if (list_length(vars) == 1) {
          pat = alloc_pat(p_var);
          var_name(pat) = (char *)list_head(vars);
          list_destroy(&vars);
        } else {
          pat = alloc_pat(p_tuple);
          tuple_val(pat) = vars;
        }

        lex_expect(RPAREN, "pattern: expected ')'.\n");
        break;
      }

    case VARIABLE:
      {
        char *var;

        var = strdup(yylval.strval);
        lex(); // Consume VARIABLE
        if (yylval.lexeme == COLON) {
          lex(); // Consume ':'
          if (yylval.lexeme != VARIABLE) {
            error("pattern: expected a variable on line %d.\n", line_num);
          }
          pat = alloc_pat(p_listcons);
          listcons_hvar(pat) = var;
          listcons_tvar(pat) = strdup(yylval.strval);
          lex(); // Consume VARIABLE
        } else {
          pat = alloc_pat(p_var);
          var_name(pat) = var;
        }

        break;
      }

    default:
      error("pattern: parse error on line %d.\n", line_num);
      return NULL; // gcc dataflow.
    }
  }

  return pat;
}

// Alternative  = Pattern "->" Expression
static clause_t *alternative(void)
{
  pattern_t *pat;
  expr_t *expr;

  if ((pat = pattern()) == NULL) {
    error("alternative: expected a pattern.\n");
  }
  lex_expect(ARROW, "alternative: expected '->'.\n");
  expr = expression();

  return alloc_clause(pat, expr);
}

// Alternatives  = Alternative Alternatives'
// Alternatives' = "|" Alternative Alternatives' | EMPTY
static list_t *alternatives(void)
{
  list_t *alts;

  alts = list_empty();
  list_cons(alternative(), alts);

  while (yylval.lexeme == ALTERNATIVE) {
    lex();
    list_cons(alternative(), alts);
  }

  return list_reverse(alts);
}

// Expression      = ConsExpr
//   LET:          | "let" Bindings "in" Expression
//   ITE:          | "if" Expression "then" Expression "else" Expression "end"
//   MATCH:        | "match" Expression "with" "{" Alternatives "}"
static expr_t *expression(void)
{
  expr_t *new;

  switch (yylval.lexeme) {
  case IF:
    {
      new = alloc_expr_node(p_ite);

      lex();
      ite_cond(new) = expression();
      lex_expect(THEN, "expression/if: expected 'then'.\n");
      ite_then(new) = expression();
      lex_expect(ELSE, "expression/if: expected 'else'.\n");
      ite_else(new) = expression();
      lex_expect(END, "expression/if: expected 'end'.\n");

      break;
  }

  case LET:
    {
      new = alloc_expr_node(p_let);

      lex();
      let_bindings(new) = bindings();
      lex_expect(IN, "expression/let: expected 'in'.\n");
      let_body(new) = expression();

      break;
    }

  case MATCH:
    {
      new = alloc_expr_node(p_match);

      lex();
      match_scrutinee(new) = expression();
      lex_expect(WITH, "expression/match: expected 'with'.\n");
      match_clauses(new) = alternatives();
      lex_expect(END, "expression/match: expected 'end'.\n");

      break;
    }

  default:
    new = consExpr();
    break;
  }

  return new;
}

// ExpressionList  = Expression ExpressionList'
// ExpressionList' = "," ExpressionList | EMPTY
static list_t *expressionList(void)
{
  list_t *exprs;

  exprs = list_empty();
  list_cons(expression(), exprs);

  while (yylval.lexeme == COMMA) {
    lex();
    list_cons(expression(), exprs);
  }

  return list_reverse(exprs);
}

//*********************************************************

// TypeList         = Type TypeList'
// TypeList'        = "," TypeList | EMPTY
static list_t *typeList(void)
{
  list_t *types;

  types = list_empty();
  list_cons(type(0), types);

  while (yylval.lexeme == COMMA) {
    lex();
    list_cons(type(0), types);
  }

  return list_reverse(types);
}

// AtomicType       = "Bool" | "Char" | "Num"
//                  | Identifier -- User defined type.
//       	  	 | "[" Type "]"
//       	  	 | "(" TypeList ")" -- Identify a parenthesised single type with that type.
// If emptyOK is true, this function may return NULL.
static type_t *atomicType(int emptyOK)
{
  type_t *new;

  switch (yylval.lexeme) {
  case BOOL:
    new = &boolT;
    lex();
    break;

  case CHAR:
    new = &charT;
    lex();
    break;

  case NUM:
    new = &numT;
    lex();
    break;

  case IDENTIFIER:
    new = alloc_type(t_user_defined);
    user_def_use_name(new) = strdup(yylval.strval);
    lex();
    break;

  case LSBRACKET:
    {
      type_t *elt_type;

      lex();
      elt_type = type(0);
      lex_expect(RSBRACKET, "atomicType: expected ']'.\n");

      new = alloc_type(t_list);
      list_elt_type(new) = elt_type;

      break;
    }

  case LPAREN:
    {
      list_t *types;

      lex();
      types = typeList();
      lex_expect(RPAREN, "atomicType: expected ')'.\n");
      if (list_length(types) == 1) {
        new = (type_t *)list_head(types);
        list_destroy(&types);
      } else {
        new = alloc_type(t_tuple);
        tuple_types(new) = types;
      }

      break;
    }

  default:
    if (emptyOK) {
      new = NULL;
    } else {
      error("atomicType: expected 'Bool'|'Char'|'Num'|Identifier|'['|'(' on line %d.\n", line_num);
      return NULL; // gcc dataflow.
    }
  }

  return new;
}

// Type             = AtomicType FnType
// FnType           = "->" Type | EMPTY
// If emptyOK is true, this function may return NULL.
static type_t *type(int emptyOK)
{
  type_t *new;

  if ((new = atomicType(emptyOK)) == NULL) {
    if (emptyOK) {
      new = NULL;
    } else {
      error("type: expected a type on line %d.\n", line_num);
      return NULL; // gcc dataflow
    }
  } else {
    while (yylval.lexeme == ARROW) {
      type_t *arg = new;
      type_t *ret;

      lex();
      ret = type(0);
    
      new = alloc_type(t_fn);
      fn_arg_type(new) = arg;
      fn_ret_type(new) = ret;
    }
  }

  return new;
}

// DataConstructor  = Identifier Types
static datacons_t *dataCons(void)
{
  datacons_t *dc;
  type_t *param;

  if (yylval.lexeme != IDENTIFIER) {
    error("dataCons: expected IDENTIFIER on line %d.\n", line_num);
    return NULL; // gcc dataflow
  }
  dc = alloc_datacons(strdup(yylval.strval));
  lex(); // Consume tag IDENTIFIER.

  dc->params = list_empty();
  while ((param = type(1)) != NULL) {
    list_cons(param, dc->params);
  };
  list_reverse(dc->params);

  return dc;
}

// TypeDesc         = DataConstructor TypeDesc'
// TypeDesc'        = "|" DataConstructor TypeDesc' | EMPTY
static list_t *typeDesc(void)
{
  list_t *dcs;

  dcs = list_empty();
  list_cons(dataCons(), dcs);
  while (yylval.lexeme == ALTERNATIVE) {
    lex(); // Consume '|'
    list_cons(dataCons(), dcs);
  }

  return list_reverse(dcs);
}

// TypeDefs         = TypeDef TypeDefs | EMPTY
// TypeDef          = "data" Identifier = TypeDesc
list_t *typedefs(void)
{
  list_t *typedefs;

  typedefs = list_empty();

  while (yylval.lexeme == DATA) {
    type_t *t;

    t = alloc_type(t_user_defined);

    lex(); // Consume "data"
    if (yylval.lexeme != IDENTIFIER) {
      error("typedef: expected IDENTIFIER on line %d.\n", line_num);
      return NULL; // gcc dataflow
    }
    user_def_datacons_name(t) = strdup(yylval.strval);
    lex(); // Consume IDENTIFIER
    lex_expect(EQUALS, "typedef: expected '='.\n");
    user_def_datacons_dcs(t) = typeDesc();
    list_cons(t, typedefs);
  }

  return typedefs;
}

//*********************************************************

// Bindings         = Binding Bindings | EMPTY
// Binding          = Variable "::" Type
//                    Variable Variables = Expression ";"
static list_t *bindings(void)
{
  list_t *bs;

  bs = list_empty();

  while (yylval.lexeme == VARIABLE) {
    binding_t *b;

    b = alloc_binding(strdup(yylval.strval));
    lex();
    lex_expect(DOUBLECOLON, "bindings: expected '::'.\n");
    b->type = type(0);

    if (yylval.lexeme == VARIABLE) {
      if (strcmp(b->name, yylval.strval) != 0) {
        error("binding: expected a clause for binding '%s', got one for '%s' instead.\n",
              b->name, yylval.strval);
      }
      lex();

      // Parse the function parameters.
      b->params = list_empty();
      while (yylval.lexeme == VARIABLE) {
        list_cons(strdup(yylval.strval), b->params);
        lex();
      }
      list_reverse(b->params);

      lex_expect(EQUALS, "binding: expected '='.\n");
      b->body = expression();

      lex_expect(SEMICOLON, "binding: expected ';'.\n");

      list_cons(b, bs);
    } else {
      error("clause: expected VARIABLE on line %d.\n", line_num);
      return NULL; // gcc dataflow
    }
  }

  return list_reverse(bs);
}

static expr_t *p_main(void)
{
  lex_expect(MAIN, "main: expected \"main\".\n");
  lex_expect(EQUALS, "main: expected '='.\n");
  return expression();
}

program_t *program(void)
{
  program_t *program;

  program = (program_t *)util_malloc(sizeof(program_t));

  program->types = typedefs();
  program->bindings = bindings();
  program->main = p_main();
  lex_expect(LEXER_EOF, "main: expected EOF\n");

  return program;
}

//*********************************************************
// Driver.

program_t *parse(void)
{
    // The parser expects the next symbol to be in the yylval var.
    lex();

    return program();
}
