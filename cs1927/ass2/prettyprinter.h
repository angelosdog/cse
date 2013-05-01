//
// The Pretty Printer.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 22/08/01 (SimpleC).
// Redone for FLAN July 2008.
//
//

#ifndef _PRETTYPRINT_H_
#define _PRETTYPRINT_H_

#include <stdio.h>

#include "parser.h"

int pp_var_i(void *data, void *info);

void pp_expr(FILE *outf, expr_t *expr, int level);
void pp_pattern(FILE *outf, pattern_t *pat);
void pp_type(FILE *outf, type_t *type);
void pp_program(FILE *outf, program_t *program);

#endif // _PRETTYPRINT_H_
