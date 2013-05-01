#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Item.h"
#include "lists.h"

int main (int argc, char* argv[]) {
  printf("Initialising list\n");
  link ls = newNode (1);
  insertNode (ls, 4);
  insertNode (ls, 3);
  insertNode (ls, 2);
  insertNode (ls, -5); 
  printf("List now contains");
  traverse (ls, printNode);
  printf("\n");
  printf("Squaring all items in list\n");
  squareAll(ls);
  printf ("List is now");
  traverse (ls, printNode);
  printf("\n");
  printf("Adding 6 to end of list\n");
  insertNode(ls, 6);
  printf("List is now");
  traverse(ls, printNode);
  printf("\n");
  printf("Counting all even items in list. Expected 3, returned %d\n", count(ls, &isEven));
  assert(count(ls, &isEven) == 3);
  printf("Counting all odd items in list. Expected 3, returned %d\n", count(ls, &isOdd));
  assert(count(ls, &isOdd) == 3);
  printf("Counting all 0's in list. Expected 0, returned %d\n", count(ls, &isZero));
  assert(count(ls, &isZero) == 0);
  printf("Counting all items greater than 3 in the list. Expected 5, returned %d\n", count(ls, &isGreaterThan3));
  assert(count(ls, &isGreaterThan3) == 5);
  printf("Counting all items less than 10 in the list. Expected 4, returned %d\n", count(ls, &isLessThan10));
  assert(count(ls, &isLessThan10) == 4);
  return 0;
}

