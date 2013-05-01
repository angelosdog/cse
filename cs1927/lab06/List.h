/* List.h
* Written for COMP1927 2012s2
*/

typedef struct _list *List;
typedef int bool;

#define TRUE (1 == 1)
#define FALSE (1 == 0)


// Returns an empty list
List createEmptyList();

// Adds an item to the list l (item a should be the
// first item in the new list). If l is empty, add should
// return a new list of length 1 containing item a
List add(List l, int a);


// Frees the list, and deletes all elements
// Should always return NULL unless unsuccessful
List freeList (List l);

// Returns TRUE if the list contains an instance of element a
bool contains(List l, int a);

// Returns the size of the list l (i.e. the number of items in the list)
int size(List l);


// Returns true if the list is in sorted order (increasing)
bool isSorted(List l);

// Copies the first n elements from the list l to the array a
void copyToArray(List l, int *a, int n);

// Returns a sorted copy of the list l
// Sorts the list using mergesort
List mergesortList(List l);

// Returns a sorted copy of the list l
// Sorts the list using quicksort
List quicksortList(List l);
