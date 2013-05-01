#ifndef HASHTABLE
#define HASHTABLE 1==1

#include "bool.h"
#include "item.h"
typedef struct _hashTable * HashTable;


//Creates a new Hash table
// @return, a new hash table
HashTable newHashTable();

// @param i, the item to be hashed and added to the hashTable
// @param h, the hashtable to be added to and returned
// @return The hashtable passed in and modified
HashTable put(Item i, HashTable h);

//Returns true if the hashTable contains a particular element
// @param i, the item to be hashed and added to the hashTable
// @param h, the hashtable to be added to and returned
// @return true if the hashtable is correct, else false
bool hashTableContains(Item i, HashTable h);

//Deletese the hash table and returns NULL
// @param H, the hashtable to be deleted
// @return NULL
HashTable deleteHashTable(HashTable h);

#endif
