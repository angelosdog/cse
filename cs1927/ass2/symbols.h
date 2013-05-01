//
// The Symbol Table library.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01 (SimpleC).
// Redone for FLAN July 2008.
//
// Keys are strings; data objects are just void pointers.
// NOTE: keys are NOT copied.
//

#ifndef _SYMBOLS_H_
#define _SYMBOLS_H_

#include "list.h"

typedef list_t symboltable_t;

// Initialise the symbol table.
symboltable_t *st_init(void);

// Insert a symbol into the table.
symboltable_t *st_insert(symboltable_t *st, char *key, void *data);

// Lookup a symbol in the table.
void *st_lookup(symboltable_t *st, char *key);

// Iterate over a symbol table.
void st_iterate(symboltable_t *st,
                int (fn)(char *key, void *data, void *info),
                void *info);

// Is this symbol table empty?
int st_isEmpty(symboltable_t *st);

// Destroy a symbol table.
void st_destroy(symboltable_t **st);

#endif // _SYMBOLS_H_
