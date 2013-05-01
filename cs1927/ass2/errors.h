//
// The Error Reporting library.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01.
//
// Functions with a variable number of arguments (...) have the same calling
// conventions as printf(3).
//

#ifndef _ERRORS_H_
#define _ERRORS_H_

//
// Toggles the generation of core dumps on errors.
// This is handy for debugging the parser.
//
void toggle_core_dumps(void);

void codegen_error(char *fmt, ...);

void parse_error(char *fmt, ...);

void error(char *fmt, ...);

#endif // _ERRORS_H_
