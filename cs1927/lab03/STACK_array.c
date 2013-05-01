#include <stdlib.h>
#include <stdio.h>
#include "Item.h"
#include "STACK.h"

static Item *s;
static int N;
static int maxSize;

void STACKinit (int maxN)  { 
  s = malloc (maxN * sizeof(Item) );
  N = 0; 
  maxSize = maxN;
}

int STACKempty(void) { 
  return N == 0; 
}

void STACKpush (Item item) {
  if(N < maxSize){
   s[N] = item; 
   N++;
  } else {
    printf("Error: Stack overflow. Stack has been completely filled up.");
    abort();
  }
}

Item STACKpop (void) {
  if (N > 0){
    N--;
    return s[N]; 
  } else {
    printf("Error: Stack underflow. Trying to pop an item off a stack which doesn't exist.\n");
    abort();
  }
}

