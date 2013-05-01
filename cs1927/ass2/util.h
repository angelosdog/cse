//
// Various utility functions without obvious homes.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01.
//

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdlib.h>

// strdup is not in the C standards...
char *strdup(const char *s);

// Centralise calls to malloc/free so we can replace them.  Due to
// flex directly calling malloc, this is slightly nasty: we directly
// rewrite the calls to malloc and free.

#ifdef BOEHM_GC

// Use the Boehm conservative collector.
#include <gc/gc.h>

#define util_memory_init GC_init()
#define util_malloc GC_malloc
#define util_realloc GC_realloc
// This works as free takes one parameter, lucky us.
#define util_free (void)

#else

// Use malloc/free from stdlib.h
#define util_memory_init
#define util_malloc malloc
#define util_realloc realloc
#define util_free free

#endif // BOEHM_GC

#endif // _UTIL_H_
