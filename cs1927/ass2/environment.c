//
// Environments for the evaluator.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//
// A stack of symbol tables.
//

#include <stdlib.h>

#include "environment.h"
#include "errors.h"
#include "prettyprinter.h"
#include "symbols.h"
#include "util.h"

typedef struct node node_t;

struct env {
    node_t *head;
};

struct node {
    symboltable_t *st;
    node_t *next;
};

env_t *env_init(void)
{
  env_t *env;
  node_t *node;

  node = (node_t *)util_malloc(sizeof(node_t));
  node->st = st_init();
  node->next = NULL;

  env = (env_t *)util_malloc(sizeof(env_t));
  env->head = node;

  return env;
}

env_t *env_new_scope(env_t **env)
{
  env_t *newEnv;

  newEnv = env_init();
  newEnv->head->next = (*env)->head;
  *env = newEnv;

  return newEnv;
}

int env_add_binding(env_t *env, char *var, void *val)
{
  int result = st_lookup(env->head->st, var) != NULL;

  st_insert(env->head->st, var, val);

  return result;
}

void *env_lookup(env_t *env, char *var)
{
  void *result;
  node_t *cur = env->head;

  while ((result = st_lookup(cur->st, var)) == NULL && cur->next != NULL) {
    cur = cur->next;
  }

  return result;
}

void env_destroy_scope(env_t **env)
{
  node_t *cur;

  cur = (*env)->head->next;
  st_destroy(&(*env)->head->st);
  util_free((*env)->head);

  if (cur) {
    (*env)->head = cur;
  } else {
    util_free(*env);
    *env = NULL;
  }
}

void env_print(FILE *outf, env_t *env, int (fn)(char *key, void *data, FILE *outf))
{
  node_t *cur;

  fprintf(outf, ">> Environment <<\n");
  for (cur = env->head; cur != NULL; cur = cur->next) {
    if (st_isEmpty(cur->st)) {
      fprintf(outf, "<< empty >>\n");
    } else {
      fprintf(outf, "  >> New scope <<\n");
      st_iterate(cur->st,
                 (int (*)(char *, void *, void *))fn, // Eek!
                 outf);
    }
  }
}
