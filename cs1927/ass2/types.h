//
// Types, type checking.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 22/08/01 (SimpleC).
// Redone for FLAN July 2008.
//

#ifndef _TYPES_H_
#define _TYPES_H_

#include "parser.h"

//*********************************************************
// Exported functions.

type_t *alloc_type(type_type_t type);

// Typecheck a program. Returns 0 on failure.
int types_check(program_t *program);

#endif // _TYPES_H_
