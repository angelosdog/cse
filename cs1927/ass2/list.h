//
// Lists.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced July 2008.
//

#ifndef _LISTS_H_
#define _LISTS_H_

typedef struct list list_t;

// Create an empty list.
list_t *list_empty(void);

// Iterate over a list. If the function returns 0, the traversal stops.
void list_iterate(list_t *list,
                  int (fn)(void *data, void *info),
                  void *info);

// Iterate over two lists in lockstep. If the function returns 0, the
// traversal stops. If 'fn' never returns 0, then 'list_zip_with'
// returns -1 if list_l is longer than list_r, 1 for
// the converse and 0 if their lengths are equal.
int list_zip_with(list_t *list_l,
                  list_t *list_r,
                  int (fn)(void *data_l, void *data_r, void *info),
                  void *info);

// Is the list empty?
int list_isEmpty(list_t *list);

// Add an element to the front of a list.
list_t *list_cons(void *elt, list_t *list);

// Add an element to the front of a list. (Creates a new list_t and
// shares the existing nodes.)
list_t *list_new_cons(void *data, list_t *list);

// Fetch the head of the list.
void *list_head(list_t *list);

// Fetch the tail of the list. (Creates a new list_t and shares the nodes.)
list_t *list_new_tail(list_t *list);

// Compute the length of the list.
int list_length(list_t *list);

// Append one list to another (destroy l1, share l2).
list_t *list_append(list_t **l1, list_t **l2);

// Append one list to another (non-destructively - copy l1, share l2).
list_t *list_append_new(list_t *l1, list_t *l2);

// Drop the first /n/ elements of the list. (Creates a new list_t and shares the nodes.)
list_t *list_drop_new(int n, list_t *list);

// Reverse the list, in-place (destructively).
list_t *list_reverse(list_t *list);

// Get the /n/th element of a list. Not efficient.
void *list_nth(list_t *list, int n);

// Sort the list, in-place (destructively). From the manpage for qsort(3):
//
// The comparison function must return an integer less than, equal to,
// or greater than zero if the first argument is considered to be respectively
// less than, equal to, or greater than the second.  If two members compare as
// equal, their order in the sorted array is undefined.
list_t *list_sort(list_t *list,
                  int (compare)(void *data_l, void *data_r));

// Destroy the list (the list_t and list nodes, but not the elements).
void list_destroy(list_t **list);

#endif // _LIST_H_
