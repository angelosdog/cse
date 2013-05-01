//
// Various utility functions without obvious homes.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01.
//

#include <stdlib.h>

#include "util.h"

char *strdup (const char *s)
{
  size_t len = strlen(s) + 1;
  void *new = util_malloc(len);

  if (new == NULL)
    return NULL;

  return (char *)memcpy(new, s, len);
}
