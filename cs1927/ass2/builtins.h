//
// Builtin functions.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//

#ifndef _BUILTINS_H_
#define _BUILTINS_H_

#include "environment.h"
#include "parser.h"
#include "symbols.h"

// The list data constructors.
extern char *listEmptyTag;
extern char *listConsTag;

// Pre-defined types.
extern type_t boolT;
extern type_t charT;
extern type_t numT;

symboltable_t *initial_type_environment(list_t *extraTypes);

// Pre-defined functions.
env_t *builtin_bindings(void);
env_t *builtin_bindings_types(void);

#endif // _BUILTINS_H_
