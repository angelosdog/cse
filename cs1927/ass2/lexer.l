%{		/* -*- C -*- */

/*
 * FLAN lexical analyzer.
 *
 * Peter Gammie, peteg@cse.unsw.edu.au
 * Commenced a long time ago.
 */
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "errors.h"
#include "util.h"

#define MAX_STRVAL_LEN 1024

int line_num = 1;
YYSTYPE yylval;
static int output_lexemes = 0;

/* Stack of looked at tokens. */
YYSTYPE *la_stack;

static void count();
static int yywrap();

int yylex();

static void print_lexeme(char *fn)
{
    if(output_lexemes) {
	switch(yylval.lexeme) {
	case CHARCONST:
	    printf("%s %d: lexeme: %d (CHARCONST)\n", fn, line_num, yylval.charval);
	    break;
	case IDENTIFIER:
	    printf("%s %d: lexeme: \"%s\" (IDENTIFIER)\n", fn, line_num, yylval.strval);
	    break;
	case NUM:
	    printf("%s %d: lexeme: %f (NUM)\n", fn, line_num, yylval.numval);
	    break;
	case VARIABLE:
	    printf("%s %d: lexeme: \"%s\" (VARIABLE)\n", fn, line_num, yylval.strval);
	    break;
	default:
	    printf("%s %d: lexeme: %s\n", fn, line_num, lex_toString(yylval.lexeme));
	}
    }
}

void toggle_output_lexemes() {
    output_lexemes = !output_lexemes;
}

void lex()
{
    if(la_stack != NULL) {
	/* Use a looked-at token. This is leaky - strval. */
	YYSTYPE *ptr = la_stack;

	yylval = *ptr;
	la_stack = ptr->next;
	util_free(ptr);
    } else {
	/* Hose the previous lexeme with some unlikely constants for debugging purposes. */
	yylval.lexeme = -1;
	yylval.strval = NULL;
	yylval.charval = 0x09;
	yylval.numval = -153.3234;

	yylval.lexeme = yylex();
    }

    print_lexeme("lex");
}

token_t lex_lookahead(int n)
{
    token_t la_lexeme = -1;
    YYSTYPE current_token, *tokens, *cur, **cur_ptr;

    if(n <= 0) {
	error("lex_lookahead: n must be positive.\n");
    }

    /* Save current token. */
    current_token.lexeme  = yylval.lexeme;
    current_token.charval = yylval.charval;
    current_token.numval  = yylval.numval;
    current_token.strval  = (yylval.strval == NULL) ? NULL : strdup(yylval.strval);
    current_token.next    = NULL;

    cur_ptr = &tokens;
    cur = NULL; /* gcc data flow. */

    while(n-- > 0) {
	lex();

	if((*cur_ptr = (YYSTYPE *)util_malloc(sizeof(YYSTYPE))) == NULL) {
	    error("lex lookahead: out of memory.");
	}
	cur = *cur_ptr;
	cur_ptr = &cur->next;

	cur->lexeme  = yylval.lexeme;
	cur->charval = yylval.charval;
	cur->numval  = yylval.numval;
	cur->strval  = (yylval.strval == NULL) ? NULL : strdup(yylval.strval);
    }

    print_lexeme("lookahead");
    la_lexeme = cur->lexeme;

    /* Patch in any previously looked at (but not this time) tokens. */
    *cur_ptr = la_stack;
    la_stack = tokens;

    /* Restore old yylval. */
    yylval = current_token;

    return la_lexeme;
}

void lex_expect(token_t lexeme, char *fmt, ...)
{
    if(yylval.lexeme == lexeme) {
	lex();
    } else {
	va_list argp;

	va_start(argp, fmt);
	parse_error(fmt, argp);
	va_end(argp);
    }
}

void lex_string_start(const char *str)
{
  yypush_buffer_state(yy_scan_string(str));
}

void lex_string_end(void)
{
  yypop_buffer_state();
}

/**
 * The table that maps escapes -> characters
 */
static char escapes[]    = {'0', '\"', '\'', '\\', 'b', 'f', 'n', 'r', 't'};
static char escape_map[] = {'\0', '\"', '\'', '\\', '\b', '\f', '\n', '\r', '\t'};

#define ESCAPES_LENGTH (sizeof(escapes) / sizeof(char))

/* Interpret a character constant. */
static void char_const()
{
/*     fprintf(stderr, "got CHAR CONST %s.\n", yytext); */

    if(yytext[1] == '\\') {
	/* Linear search the escapes list. */
	char ch = yytext[2];
	int i;

	for(i = 0; i < ESCAPES_LENGTH && escapes[i] != ch; i++);

	if(i >= ESCAPES_LENGTH) {
	    error("lexer/char_const: unknown escape: \"%s\".\n", yytext);
	}

	yylval.charval = escape_map[i];
    } else {
	yylval.charval = yytext[1];
    }
}

%}

/* Do our own memory management. */
%option noyyalloc noyyrealloc noyyfree

/* Introduce a lexer state. */
%s COMMENT

%%

<INITIAL>--.*$				{ count(); }

<INITIAL>"{-"	       	BEGIN(COMMENT);

<COMMENT>[^-\n]*        {}
<COMMENT>"-"+[^-}\n]*   {}
<COMMENT>\n		{ count(); }
<COMMENT>"-"+"}"	BEGIN(INITIAL);

<INITIAL>"|"				{ return(ALTERNATIVE);		}
<INITIAL>","				{ return(COMMA);		}
<INITIAL>"::"				{ return(DOUBLECOLON);		}
<INITIAL>";"				{ return(SEMICOLON);		}
<INITIAL>"("				{ return(LPAREN);		}
<INITIAL>")"				{ return(RPAREN);		}
<INITIAL>"[]"				{ return(EMPTYLIST);		}
<INITIAL>"["				{ return(LSBRACKET);		}
<INITIAL>"]"				{ return(RSBRACKET);		}

<INITIAL>"||"				{ return(OROP);			}
<INITIAL>"&&"				{ return(ANDOP);		}

<INITIAL>"="				{ return(EQUALS);		}
<INITIAL>"=="				{ return(EQOP);			}
<INITIAL>"!="				{ return(NEOP);			}
<INITIAL>"<"				{ return(LTOP);			}
<INITIAL>"<="				{ return(LEOP);			}
<INITIAL>">"				{ return(GTOP);			}
<INITIAL>">="				{ return(GEOP);			}

<INITIAL>":"				{ return(COLON);		}
<INITIAL>"+"				{ return(PLUS);			}
<INITIAL>"-"				{ return(MINUS);		}
<INITIAL>"*"				{ return(MULT);			}
<INITIAL>"/"				{ return(DIVIDE);		}
<INITIAL>"%"				{ return(MODULUS);		}

<INITIAL>"!"				{ return(NEGOP);		}
<INITIAL>"->"				{ return(ARROW);		}

<INITIAL>"data"				{ return(DATA);			}
<INITIAL>"Bool"				{ return(BOOL);			}
<INITIAL>"Char"				{ return(CHAR);			}
<INITIAL>"Num"				{ return(NUM);			}

<INITIAL>"error"			{ return(ERROR);		}
<INITIAL>"if"				{ return(IF);			}
<INITIAL>"then"				{ return(THEN);			}
<INITIAL>"else"				{ return(ELSE);			}
<INITIAL>"in"				{ return(IN);			}
<INITIAL>"let"				{ return(LET);			}
<INITIAL>"main"				{ return(MAIN);			}
<INITIAL>"match"			{ return(MATCH);		}
<INITIAL>"with"				{ return(WITH);			}
<INITIAL>"end"				{ return(END);			}

<INITIAL>"False"			{ return(FALSE);		}
<INITIAL>"True"				{ return(TRUE);			}

<INITIAL>[ \t\n\r]+			{ /* whitespace */ count();	}

<INITIAL>'(\\.|[^\\'])'                 { char_const(); return(CHARCONST); }

<INITIAL>[0-9]+(\.[0-9]+)?		{ yylval.numval = atof(yytext); return(NUMCONST); }
<INITIAL>[A-Z][A-Za-z0-9_]*		{ yylval.strval = (char *)yytext; return(IDENTIFIER); }
<INITIAL>[a-z][A-Za-z0-9_]*		{ yylval.strval = (char *)yytext; return(VARIABLE); }

.                                       { error( "lexer: line %d: unrecognized lexeme starting with character \"%s\".\n", line_num, yytext); }

%%

/* Maintain the line counter
 * (called whenever a newline could appear in the input)
 * FIXME: does this interact with lookahead?
 */
static void count()
{
    int i;

    for(i = 0; yytext[i] != '\0'; i++)
	if(yytext[i] == '\n')
	    line_num++;
}

/* Trivial yywrap() to keep lex happy (only ever one source file) */
static int yywrap()
{
    yylval.lexeme = LEXER_EOF;
    return 1;
}

/* Memory management functions. */
void *yyalloc(yy_size_t bytes) {
  return util_malloc(bytes);
}

void *yyrealloc(void *ptr, yy_size_t bytes) {
  return util_realloc(ptr, bytes);
}

void yyfree(void * ptr) {
  util_free(ptr);
}

/* Dummy definition to suppress warnings about unused static definitions. */
typedef void (*dummy_function) ();
dummy_function ada_flex_use[] = 
{ 
  (dummy_function) yyrealloc, (dummy_function) yyunput
};
