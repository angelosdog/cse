//
// Lists.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//

#include <stdlib.h>

#include "list.h"
#include "util.h"

typedef struct node node_t;

struct list {
    node_t *head;
};

struct node {
    void *data;
    node_t *next;
};

list_t *list_empty(void)
{
  list_t *l;

  l = (list_t *)util_malloc(sizeof(list_t));
  l->head = NULL;

  return l;
}

list_t *list_cons(void *elt, list_t *l)
{
  node_t *node;

  node = (node_t *)util_malloc(sizeof(node_t));

  node->data = elt;
  node->next = l->head;
  l->head = node;

  return l;
}

void list_iterate(list_t *l,
                  int (fn)(void *data, void *info),
                  void *info)
{
  node_t *node;

  for (node = l->head; node != NULL; node = node->next) {
    if (!fn(node->data, info)) {
      break;
    }
  }
}

int list_zip_with(list_t *list_l,
                  list_t *list_r,
                  int (fn)(void *data_l, void *data_r, void *info),
                  void *info)
{
  node_t *node_l;
  node_t *node_r;

  for (node_l = list_l->head, node_r = list_r->head
        ; node_l != NULL && node_r != NULL
        ; node_l = node_l->next, node_r = node_r->next) {
    if (!fn(node_l->data, node_r->data, info)) {
      break;
    }
  }

  return
    node_l == NULL
      ? (node_r == NULL ? 0 : 1)
      : -1;

}

int list_isEmpty(list_t *l)
{
    return l->head == NULL;
}

void *list_head(list_t *l)
{
  return l->head->data;
}

list_t *list_new_cons(void *elt, list_t *l)
{
  list_t *new;

  new = list_empty();
  new->head = l->head;
  list_cons(elt, new);

  return new;
}

list_t *list_new_tail(list_t *list)
{
  list_t *tail;

  tail = list_empty();
  tail->head = list->head->next;

  return tail;
}

int list_length(list_t *l)
{
  int len = 0;
  node_t *node;

  for (node = l->head; node != NULL; node = node->next) {
    len++;
  }

  return len;
}

list_t *list_append(list_t **l1, list_t **l2)
{
  node_t **cur = &(*l1)->head;

  while (*cur != NULL) {
    cur = &(*cur)->next;
  }
  *cur = (*l2)->head;

  util_free(*l2);
  *l2 = NULL;

  return *l1;
}

list_t *list_append_new(list_t *l1, list_t *l2)
{
  list_t *l = list_empty();
  node_t **curl = &l->head;
  node_t **curl1 = &l1->head;

  while (*curl1 != NULL) {
    *curl = (node_t *)util_malloc(sizeof(node_t));

    (*curl)->data = (*curl1)->data;
    curl = &(*curl)->next;
  }
  *curl = l2->head;

  return l;
}

list_t *list_drop_new(int n, list_t *l)
{
  list_t *result;
  node_t *cur;

  result = list_empty();
  for (cur = l->head; n > 0 && cur != NULL; cur = cur->next) {
    n--;
  }

  result->head = cur;

  return result;
}

list_t *list_reverse(list_t *l)
{
  node_t *rev;
  node_t *node;

  node = l->head;
  rev = NULL;
  while (node != NULL) {
    node_t *tmp;

    tmp = node->next;
    node->next = rev;
    rev = node;
    node = tmp;
  }

  l->head = rev;

  return l;
}

void *list_nth(list_t *l, int n)
{
  node_t *cur;

  cur = l->head;
  while (n-- > 0 && cur != NULL) {
    cur = cur->next;
  }

  return cur != NULL
    ? cur->data
    : NULL;
}

void list_sort_partition_aux(node_t *l,
                	     void *pivot,
                	     int (compare)(void *data_l, void *data_r),
                	     node_t **lout, node_t **rout)
{
  node_t *cur;

  *lout = *rout = NULL;

  cur = l;
  while (cur != NULL) {
    node_t *t = cur->next;

    if (compare(cur->data, pivot) < 0) {
      cur->next = (*lout);
      *lout = cur;
    } else {
      cur->next = (*rout);
      *rout = cur;
    }

    cur = t;
  }
}

node_t *list_sort_aux(node_t *l, 
                      int (compare)(void *data_l, void *data_r))
{
  if (l == NULL) {
    return NULL;
  } else {
    node_t *left;
    node_t *right;
    node_t **cur;

    // The pivot is the first element of the list.
    list_sort_partition_aux(l->next, l->data, compare, &left, &right);
    left = list_sort_aux(left, compare);
    right = list_sort_aux(right, compare);

    // Append.
    l->next = right;
    
    cur = &left;
    while (*cur != NULL) {
      cur = &(*cur)->next;
    }
    *cur = l;

    return left;
  }
}

// Use quicksort.
list_t *list_sort(list_t *list,
                  int (compare)(void *data_l, void *data_r))
{
  list->head = list_sort_aux(list->head, compare);  

  return list;
}

// Destroy the list (but not the elements).
void list_destroy(list_t **l)
{
  node_t *node;

  node = (*l)->head;
  while (node != NULL) {
    node_t *tmp;

    tmp = node->next;
    util_free(node);
    node = tmp;
  }

  util_free(*l);
  *l = NULL;
}
