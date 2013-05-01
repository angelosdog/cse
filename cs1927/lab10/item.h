#ifndef ITEM
#define ITEM 1==1

#include "bool.h"

typedef char* Item;

//Hashes a given item
//  @param i, the item to be hashed
//  @return, a hash of the given item (should be reasonably unique)
int hash(Item i);

//compares the two items, and returns true if they are the same
//  @param a, the first element to compare
//  @param b, the second element to compare
//  @return, true if a equals b, else false
bool compare(Item a, Item b);

#endif
