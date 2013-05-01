#ifndef HASHTABLE
#define HASHTABLE 1==1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashTable.h"
#include "list.h"
#include "item.h"
#include "bool.h"

#define NUM_BUCKETS 255

typedef struct _hashTable * HashTable;

struct _hashTable{ 
    List* buckets;
    int size;
};

//Creates a new hash table
//   @return a new hash table
HashTable newHashTable() {
    HashTable new = malloc(sizeof(struct _hashTable));
    new->buckets = malloc(sizeof(List) * NUM_BUCKETS);
    int i = 0;
    for (i = 0; i < NUM_BUCKETS; i++) {
        new->buckets[i] = newList();
    }
    new->size = NUM_BUCKETS;
    return new;
}

//Adds the item to the hashTable given
// @param i, the item to be hashed and added to the hashTable
// @param h, the hashtable to be added to and returned
// @return The hashtable passed in and modified
HashTable put(Item i, HashTable h) {
    h->buckets[hash(i) % NUM_BUCKETS] = add(i, h->buckets[hash(i) % NUM_BUCKETS]);
    return h;
}


//Returns true if the hashTable contains a particular element
// @param i, the item to be hashed and added to the hashTable
// @param h, the hashtable to be added to and returned
// @return true if the hashtable is correct, else false
bool hashTableContains(Item i, HashTable h) {
    if (listContains(i, h->buckets[hash(i) % NUM_BUCKETS])) return true;
    return false;
}

HashTable deleteHashTable(HashTable new) {
    int i;
    for (i = 0; i < NUM_BUCKETS; i++){
        deleteList(new->buckets[i]);
    }
    free(new);
    return NULL;
}

#endif
