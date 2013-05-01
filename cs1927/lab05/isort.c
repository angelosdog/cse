#include <stdio.h>
#include <stdlib.h>

// Function prototypes

void insertionSort (int sequence[], int numElements);
void swap (int *x, int *y);
void printArray (int sequence[], int low, int high);



/* Insertion Sort Algorithm */
void insertionSort(int sequence[], int numElements) {
    int i, j;

    for (i = 1; i < numElements; i++) {
        for (j = i; j > 0; j--) {
            if (sequence[j] < sequence[j - 1]) {
                swap(&sequence[j], &sequence[j - 1]);
            } else {
                break;
            }
        }
    }
}   

/* Auxiliary Routines */ 

// Print the contents of an array of integers sequence
// between the indices low and high
void printArray(int sequence[], int low, int high) {
    int i;

    for (i = low; i <= high; i++) {
        printf(" %d", sequence[i]);
    }
    printf("\n");
}

// Swap two integer values using pass by reference
void swap(int *x, int *y) {
    int temp;

    temp = *x;
    *x = *y;
    *y = temp;
}

// This is where it all happens!
int main(int argc, char *argv[]) {
    int i, numElements;
    int *sequence;

    // Obtain number of elements and allocate memory to store them
    fprintf(stderr, "Enter number of elements\n");
    scanf("%d", &numElements);
    sequence = (int *) malloc(numElements * sizeof(int)); // Array of elements

    // Obtain data and store them in array sequence
    fprintf(stderr, "Enter elements to be sorted\n");
    for (i = 0; (i < numElements) && (scanf("%d", &sequence[i]) != EOF); i++)
        ; // Do nothing - it's all done in the for statement!
    numElements = i; // Just in case user entered fewer items

    printf("\nArray before sorting\n");
    printArray(sequence, 0, numElements - 1);

    printf("\nInsertion sort\n\n");
    insertionSort(sequence, numElements);

    printf("Array after sorting\n");
    printArray(sequence, 0, numElements - 1);

    return 0;
}
