//
// FLAN main file.
// Tie together the lexer, parser, type checker, pretty printer.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//
//

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "errors.h"
#include "eval.h"
#include "flange.h"
#include "lexer.h"
#include "list.h"
#include "parser.h"
#include "prettyprinter.h"
#include "types.h"
#include "util.h"
#include "values.h"

void usage_error(char *progname, char *fmt, ...)
{
  va_list argp;

  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
  fprintf(stdout,
            "Usage: %s [-c] [-e] [-f] [-g] [-l] [-p] [-r] [-t] [-w] [sourcefile]\n\n\
  -c   Cause a segmentation violation on error. (default: don't)\n\
  -e   Don't evaluate \"main\". (default: do)\n\
  -f   Use the first-order strict evaluator (default: use the higher-order strict evaluator)\n\
  -g   Treat \"main\" as a FLANGE picture. (default: don't)\n\
  -l   Evaluate the program lazily. (default: evaluate it strictly)\n\
  -p   Write out the parsed version of the program. (default: don't)\n\
  -r   Print out the top-level of an evaluated data constructor value as a tree. (default: don't)\n\
  -t   Don't type check the program. (default: do). THIS IS UNSAFE.\n\
  -w   Write out scanned lexemes. (default: don't)\n\
  \n",
          progname);

  exit(1);
}

int main(int argc, char *argv[]) {
  char *sourcefile = NULL;
  program_t *program;
  int i;
  int eval = 1;
  int flange = 0;
  int type_check = 1;
  int pretty_print = 0;
  value_t *(*evaluate_program)(env_t *env, program_t *program) = evaluate_program_hos;

  // Open the first argument as the source file.
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      // Option.
      switch (tolower(argv[i][1])) {
      case 'c':
        toggle_core_dumps();
        break;
      case 'e':
        eval = !eval;
        break;
      case 'f':
        evaluate_program = evaluate_program_fos;
        break;
      case 'g':
        flange = !flange;
        break;
      case 'l':
        evaluate_program = evaluate_program_hol;
        break;
      case 'p':
        pretty_print = !pretty_print;
        break;
      case 'r':
        toggle_tree_printer();
        break;
      case 't':
        type_check = !type_check;
        break;
      case 'w':
        toggle_output_lexemes();
        break;
      default:
        usage_error(argv[0],
                    "%s: unknown option: \"%s\".\n",
                    argv[0],
                    &(argv[i][0]));
      }
    } else {
      // Filename.
      if (sourcefile == NULL) {
        sourcefile = argv[i];
      } else {
        usage_error(argv[0],
                    "%s: more than one filename specified: \"%s\".\n",
                    argv[0],
                    argv[i]);
      }
    }
  }

  // Open source file.
  if (sourcefile == NULL) {
    yyin = stdin;
    sourcefile = "stdin";
  } else {
    if ((yyin = fopen(sourcefile, "r")) == NULL) {
      // file open error
      fprintf(stdout, "%s: cannot open file: %s\n", argv[0], sourcefile);
      exit(1);
    }
  }

  // Initialise the memory allocator (if we need to).
  util_memory_init;

  // Parse the program.
  program = parse();
  fclose(yyin);

  if (pretty_print) {
    pp_program(stdout, program);
  }

  if (eval && flange) {
    // Let FLANGE do the type checking and evaluation.
    graphics_t *g;

    g = graphics_init(sourcefile, "image.svg");
    flange_render(type_check, g, program, evaluate_program);
    graphics_end(&g);
  } else {
    // Type check the program.
    if (type_check && !types_check(program)) {
      fprintf(stdout, " ** Aborting on type error.\n");
      exit(1);
    }

    // Switch off buffering on stdout so we get a better observation of laziness.
    setvbuf(stdout, (char *)NULL, _IONBF, 0);

    // Execute the program.
    if (eval) {
      print_value(stdout, evaluate_program(builtin_bindings(), program));
      fprintf(stdout, "\n");
    }
  }

  return 0;
}
