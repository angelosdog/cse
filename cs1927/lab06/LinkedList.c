#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List.h"

#define SWAP(A, B) {int t; t = A; A = B; B = t;}

typedef struct _list{
    int item;
    List next;
} list;

List merge(List l, List middle);
void mergesort(int a[], int left, int right);
void quicksort(int a[], int left, int right);
void printList(List l);

// Returns an empty list
List createEmptyList(){
    return NULL;
}

// Adds an item to the list l (item a should be the
// first item in the new list). If l is empty, add should
// return a new list of length 1 containing item a
List add(List l, int a){
    List temp = l;
    l = (List)malloc(sizeof(struct _list));
    l->item = a;
    l->next = temp;
    return l;
}


// Frees the list, and deletes all elements
// Should always return NULL unless unsuccessful
List freeList (List l){
    List temp = l;
    while(l != NULL){
        l = l->next;
        free(temp);
        temp = l;
    }
    return NULL;
}

// Returns TRUE if the list contains an instance of element a
bool contains(List l, int a){
    while(l != NULL && l->item != a){
        l = l->next;
    }
    if (l == NULL){
        return FALSE;
    } else {
        return TRUE;
    }
}

// Returns the size of the list l (i.e. the number of items in the list)
int size(List l){
    int count = 0;
    while(l != NULL){
        count++;
        l = l->next;
    }
    return count;
}


// Returns true if the list is in sorted order (increasing)
bool isSorted(List l){
    //List with 1 element
    if (l->next == NULL){
        return TRUE;
    } else {
        while(l->next != NULL && l->item <= l->next->item){
            l = l->next;
        }
    }
    if (l->next == NULL){
        return TRUE;
    } else {
        return FALSE;
    }
}

// Copies the first n elements from the list l to the array a
void copyToArray(List l, int *a, int n){
    int i = 0;
    assert(size(l) >= n);
    while (i < n){
        a[i] = l->item;
        i++;
        l = l->next;
    }
}

// Returns a sorted copy of the list l
// Sorts the list using mergesort
List mergesortList(List l){
    printList(l);
    int count = 0;
    List middle = l;
    List middlePrev = l;
    if(l->next != NULL){
        while(count < size(l)/2){
            middlePrev = middle;
            count++;
            middle = middle->next;
        }
        middlePrev->next = NULL;
        l = mergesortList(l);
        middle = mergesortList(middle);
        printf("Pre merge\n");
        printList(l);
        printList(middle);
        printf("\n");
        l = merge(l, middle);
    }
        printf("Post merge\n");
        printList(l);
        printf("\n");

    return l;
}

List merge(List l, List middle){
    List left = l;
    List temp = l;
    int mergeCount = 0;
    while (left != NULL && middle != NULL){
        printf("l -> ");printList(l);
        printf("temp -> ");printList(temp);
        printf("left -> ");printList(left);
        printf("middle -> ");printList(middle);
        printf("\n");
        if (left->item > middle->item){
            printf("RIGHT\n");
            if (mergeCount == 0){
                l = middle;
                temp = middle;
            } else {
                temp->next = middle;
                temp = temp->next;
            }
            middle = middle->next;
        } else {
            printf("LEFT\n");
            if(mergeCount != 0){
                temp->next = left;
                temp = temp->next;
            }
            left = left->next;

        }
        mergeCount++;
    }
    printf("l -> ");printList(l);
    printf("temp -> ");printList(temp);
    printf("left -> ");printList(left);
    printf("middle -> ");printList(middle);
    printf("\n");
    temp = l;
    while(temp->next != NULL){
        temp = temp->next;
    }
    if(left != NULL){
        temp->next = left;
    }
    if(middle != NULL){
        temp->next = middle;
    }
    return l;
}

// Returns a sorted copy of the list l
// Sorts the list using quicksort
List quicksortList(List l){
    if(l == NULL){
        return l;
    }
    if(l->next != NULL){
        printf("\nTrying to sort\n");printList(l);
        List pivot = l;
        int median = size(l)/2;
        List left = l;
        List pivotPrev= l;
        int traverseCount = 0;
        while(traverseCount < median){
            traverseCount++;
            pivotPrev = pivot;
            pivot = pivot->next;
        }
        if(pivot->next == NULL){
            return l;
        }
        pivotPrev->next = NULL;
        List leftPrev = left;
        List right = pivot->next;
        List rightPrev = right;
        List leftNext;
        List rightNext;
        while(left != NULL || right !=NULL){

            while(left != NULL && left->item < pivot->item){
                leftPrev = left;
                left = left->next;
            }
            while(right != NULL && right->item > pivot->item){
                rightPrev = right;
                right = right->next;
            }
            if(left != NULL && right != NULL){
                if(left == l && right != pivot->next){
                    l = right;
                    rightNext = right->next;
                    right->next = left->next;
                    left->next = rightNext;
                    rightPrev->next = left;
                    leftPrev = l;
                } else if(right == pivot->next && left != l){
                    pivot->next = left;
                    rightNext = right->next;
                    right->next = left->next;
                    leftPrev->next = right;
                    left->next = rightNext;
                    rightPrev = pivot;
                } else if (right == pivot->next && left == l){
                    l = right;
                    pivot->next = left;
                    leftNext = left->next;
                    left->next = right->next;
                    right->next = leftNext;
                } else {
                    leftNext = left->next;
                    rightNext = right->next;
                    leftPrev->next = right;
                    rightPrev->next = left;
                    left->next = rightNext;
                    right->next = leftNext;
                }
            } else if (left == NULL && right != NULL){
                if (right == pivot->next){
                    pivot->next = right->next;
                }
                leftPrev->next = right;
                rightPrev = right->next;
            } else if (left != NULL && right == NULL){
                if (left == l){
                    l = left->next;
                }
                rightPrev->next = left;
                leftPrev = left->next;
            }
            if(left != NULL){
                leftPrev = left;
                left = left->next;
            }
            if(right != NULL){
                rightPrev = right;
                right = right->next;
            }

        }
        left = l;
        right = pivot->next;
            printf("l -> ");printList(l);
            printf("left -> ");printList(left);
            printf("pivot -> %d\n", pivot->item); 
            printf("right -> ");printList(right);
        left = quicksortList(left);
        right = quicksortList(right);
        printf("\nMashing back together\n");
        printf("left-> ");printList(left);
        printf("pivot -> ");printList(pivot);
        l = left;
        if(left != NULL){
            while(left->next != NULL){
                left = left->next;
            }
            left->next = pivot;
            pivot->next = right;
        } else {
            pivot->next = right;
        }
        printf("l -> ");printList(l);
        if (l == pivot->next || l == NULL){
            l = pivot;
        }
    }
    printf("\nReturning -> ");
    printList(l);
    return l;
}

void printList(List l){
    while(l != NULL){
        printf("%d->", l->item);
        l = l->next;
    }
    printf("X\n");
}
