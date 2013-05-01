#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "allocator.h"

typedef unsigned char byte;

int main (int argc, char* argv[]){
  printf("Initialising suballocation");
  allocator_init(1000);
  printf("\nAttempting to initialise a second time\n");
  allocator_init(5);

  printf("Testing allocator_malloc function\n\n");
  printf("Trying to allocate a memory block which is larger than the total size of memory\n");
  byte* section1 = allocator_malloc(1025);
  printf("Expected a memory pointer to NULL. Received %p\n\n", section1);
  assert(section1 == NULL);

  section1 = allocator_malloc(15);
  
  byte* section2 = allocator_malloc(15);
  assert(section2 != NULL);
  byte* section3 = allocator_malloc(15);
  assert(section3 != NULL);
  byte* section4 = allocator_malloc(15);
  assert(section4 != NULL);
  allocator_free(section3);
  allocator_free(section2);
  allocator_free(section1);
  allocator_free(section4);
  allocator_end();
  return EXIT_SUCCESS;
}
