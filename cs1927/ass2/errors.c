//
// The Error Reporting library.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01.
//

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

static int dump_core;

void toggle_core_dumps()
{
    dump_core = !dump_core;
}

void parse_error(char *fmt, ...)
{
    va_list argp;

    va_start(argp, fmt);
    fprintf(stdout, "Parse error: %d: got lexeme: %s\n",
            line_num,
            lex_toString(yylval.lexeme));
    vfprintf(stdout, fmt, argp);
    va_end(argp);

    if (dump_core && *(int *)NULL) {
        printf("should crash.\n");
    }

    exit(1);
}


void error(char *fmt, ...)
{
    va_list argp;

    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);

    if (dump_core && *(int *)NULL) {
        printf("should crash.\n");
    }

    exit(1);
}
