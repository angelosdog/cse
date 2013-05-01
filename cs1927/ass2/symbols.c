//
// The Symbol Table library.
//
// Cheap-and-cheerful linked lists.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced 21/08/01.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "symbols.h"
#include "util.h"

typedef struct {
  char *key;
  void *data;
} keydata_t;

symboltable_t *st_init(void)
{
  return list_empty();
}

symboltable_t *st_insert(symboltable_t *st, char *key, void *data)
{
    keydata_t *kd;

    kd = (keydata_t *)util_malloc(sizeof(keydata_t));

    kd->key = key;
    kd->data = data;

    return list_cons(kd, st);
}

// A small closure for lookups.
typedef struct closure {
  char *key;
  void *value;
} stl_closure_t;

// This is a hack to avoid defining a closure for the auxiliary function.
int st_lookup_aux(char *key, void *data, void *info)
{
  stl_closure_t *c = (stl_closure_t *)info;

  if (strcmp(c->key, key) == 0) {
    c->value = data;
    return 0;
  } else {
    return 1;
  }
}

void *st_lookup(symboltable_t *st, char *key)
{
  stl_closure_t c;

  c.key = key;
  c.value = NULL;

  st_iterate(st, st_lookup_aux, &c);

  return c.value;
}

typedef struct {
  int (*fn)(char *key, void *data, void *info);
  void *info;
} st_iterate_info_t;

int st_iterate_aux(void *data, void *info)
{
  keydata_t *kd = (keydata_t *)data;
  st_iterate_info_t *sii = (st_iterate_info_t *)info;

  return sii->fn(kd->key, kd->data, sii->info);
}

void st_iterate(symboltable_t *st,
                int (fn)(char *key, void *data, void *info),
                void *info)
{
  st_iterate_info_t sii;

  sii.fn = fn;
  sii.info = info;

  list_iterate(st, st_iterate_aux, &sii);
}

int st_isEmpty(symboltable_t *st)
{
  return list_isEmpty(st);
}

static int st_destroy_i(void *data, void *info)
{
  keydata_t *kd = (keydata_t *)data;

  util_free(kd);

  return 1;
}

void st_destroy(symboltable_t **st)
{
  list_iterate(*st, st_destroy_i, NULL);
  list_destroy(st);
}
