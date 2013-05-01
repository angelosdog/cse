#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int i, numElements;

  fprintf (stderr, "Number of samples: ");
  scanf ("%d", &numElements);
  
  // Output number of samples - it is required by the sort program
  printf ("%d\n", numElements);
  
  // Generate random data
  srandom (time (NULL)); // Seed random number generator
  for (i = 0; i < numElements; i++) {// Randomly generate data
    printf("%d\n", (int) (1000 * (1.0 * random ()/RAND_MAX)));
  }
  return 0;
}
