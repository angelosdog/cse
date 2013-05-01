#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "List.h"

#define N      10
#define NTESTS 10

void genrand(int *a, int n) {
   int i;
   for (i = 0; i < n; i++) {
      a[i] = rand() % 100;
   }
}

List mklist(int *a, int n) {
   int i;
   List res = createEmptyList();
   for (i = n-1; i >= 0; i--) {
      res = add(res, a[i]);
   }

   return res;
}

int equal(List list, int *a, int n) {
   int r[N], i;
   copyToArray(list, r, n);
   for (i = 0; i < n; i++) {
      if (r[i] != a[i]) {
         return 0;
      }
   }

   return 1;
}

int intcmp(const void* a, const void* b) {
   return *(int*)a - *(int*)b;
}

int main () {
   setbuf(stdout, NULL);
   setbuf(stderr, NULL);

   printf("Testing we can create a list...\n");
   List l = add(createEmptyList(), 200);

   assert(size(l) == 1);
   assert(contains(l, 200));

   printf("Passed\n");
   printf("Testing we can add more numbers to the list...\n");
   l = add(l, 150);

   assert(contains(l, 200));
   assert(contains(l, 150));
   assert(size(l) == 2);

   l = add(l, 100);

   assert(contains(l, 200));
   assert(contains(l, 150));
   assert(contains(l, 100));
   assert(size(l) == 3);

   l = add(l, 60);

   assert(contains(l, 200));
   assert(contains(l, 150));
   assert(contains(l, 100));
   assert(contains(l, 60));
   assert(size(l) == 4);

   l = add(l, -10);

   assert(contains(l, 200));
   assert(contains(l, 150));
   assert(contains(l, 100));
   assert(contains(l, 60));
   assert(contains(l, -10));
   assert(size(l) == 5);

   l = add(l, -200);

   assert(contains(l, 200));
   assert(contains(l, 150));
   assert(contains(l, 100));
   assert(contains(l, 60));
   assert(contains(l, -10));
   assert(contains(l, -200));
   assert(size(l) == 6);

   l = add(l, 40);

   assert(contains(l, 200));
   assert(contains(l, 150));
   assert(contains(l, 100));
   assert(contains(l, 60));
   assert(contains(l, -10));
   assert(contains(l, -200));
   assert(contains(l, 40));
   assert(size(l) == 7);

   printf("Passed\n");
   printf("Testing we can copy the list to an array of given size...\n");

   int array[7];

   copyToArray(l, array, size(l));

   assert(array[0] == 40);
   assert(array[1] == -200);
   assert(array[2] == -10);
   assert(array[3] == 60);
   assert(array[4] == 100);
   assert(array[5] == 150);
   assert(array[6] == 200);

   printf("Passed\n");
   printf("Testing we can have more than one instance of a list...\n");

   List l2 = add(add(add(add(add(add(add(createEmptyList(), 200), 150), 100), 60), -10), -200), 40);

   int array2[7];

   copyToArray(l, array2, size(l));

   assert(array2[0] == 40);
   assert(array2[1] == -200);
   assert(array2[2] == -10);
   assert(array2[3] == 60);
   assert(array2[4] == 100);
   assert(array2[5] == 150);
   assert(array2[6] == 200);

   printf("Passed\n");
   printf("Testing the mergesort function sorts...\n");
   l = mergesortList(l);

   copyToArray(l, array, size(l));

   assert(array[0] == -200);
   assert(array[1] == -10);
   assert(array[2] == 40);
   assert(array[3] == 60);
   assert(array[4] == 100);
   assert(array[5] == 150);
   assert(array[6] == 200);

   printf("Passed\n");
   printf("Testing the quicksort function sorts...\n");
   l2 = quicksortList(l2);
   copyToArray(l2, array2, size(l2));

   assert(array2[0] == -200);
   assert(array2[1] == -10);
   assert(array2[2] == 40);
   assert(array2[3] == 60);
   assert(array2[4] == 100);
   assert(array2[5] == 150);
   assert(array2[6] == 200);

   printf("Passed\n");
   printf("SUCCESS!! Great job, maybe write some tests on your own??\n");
   printf("Cause that would make you even more AWESOME!!\n");

   srand(time(NULL));
   int test;
   for (test = 0; test < NTESTS; test++) {
      int arr[N];
      genrand(arr, N);
      List ql = mklist(arr, N);
      List ml = mklist(arr, N);
      ql = quicksortList(ql);
      ml = mergesortList(ml);
      qsort(arr, N, sizeof(int), intcmp);
      assert(equal(ql, arr, N));
      assert(equal(ml, arr, N));

      freeList(ql);
      freeList(ml);

      printf(".");
      if (test % 80 == 79) {
         printf("\n");
      }
   }
   printf("\n");

   return EXIT_SUCCESS;
}
