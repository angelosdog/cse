#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "allocator.h"

#define FREE 0x1337BEEF
#define USED 0xDEADBEEF
#define HEADER_SIZE sizeof(struct _header)
#define FALSE 0
#define TRUE 1

#define MERGE_WITH_NEXT_BLOCK 1
#define CANNOT_MERGE 0
#define MERGE_WITH_PREV_BLOCK -1

#define PREV_SIZE free_list_ptr->prev->size
#define CURR_SIZE free_list_ptr->size
#define NEXT_SIZE free_list_ptr->next->size

#define NEXT free_list_ptr->next
#define PREV free_list_ptr->prev
#define CURR free_list_ptr

typedef unsigned char byte;
static byte* memory = NULL;

typedef struct _header* Header;

typedef struct _header {
    u_int32_t magicNumber;
    u_int32_t size;
    Header next;
    Header prev;
} header;

Header CURR = NULL;

/*
   If n is a power of 2, return n.
   Otherwise find the next power of 2 after n.
 */
u_int32_t nextPowerOf2(u_int32_t n);

void partition(void);

void markAssigned(void);

void findLocationOfFree(Header freeAddress);

//Returns FALSE if no merge is possible. If merge is possible, perform one merge and return TRUE
int merge(void);

int checkMerge(void);

void allocator_init(u_int32_t size){
    if (size == 0){
        printf("0 is an invalid size. Aborting program\n");

        abort();
    }
    if(memory == NULL){
        printf("Input size was %d.", size);
        size = nextPowerOf2(size);
        printf("Size has been raised to the closest power of two, which is %d.\n", size);
        memory = (byte*)malloc(size);

        //printf("Address of memory is %p\n", &memory);
        assert(memory != NULL);

        CURR = (Header)memory;
        //printf("free_list_ptr points to %p\n", CURR);

        CURR->magicNumber = FREE;
        //printf("Magic number is %d\n", CURR->magicNumber);

        CURR_SIZE = size;
        NEXT = (Header)memory;
        //printf("free_list_ptr->next points to %p\n", NEXT);
        PREV = (Header)memory;
        //printf("free_list_ptr->prev points to %p\n", PREV);


    } else {
        printf("Suballocator memory already initilised\n\n");
    }
}

/* 
   Kind a long algorithm, but we take log base 2 of the size
   to determine the exponent of two required to reach that size
   The ceil(size) function will return the logarithm to the nearest
   integer greater than size if size is not already an int (ie.
   not a power of 2. We then raise 2 to the power of ceil(size)
   which will give the next power of two after size if size is
   not a power of 2.
 */ 

u_int32_t nextPowerOf2(u_int32_t n){
    return (u_int32_t)pow(2, ceil(log((double)n)/log(2)));
}

void *allocator_malloc(u_int32_t n){
    u_int32_t sizeRequired = nextPowerOf2(HEADER_SIZE + n);
    Header currentNode = CURR;
    int loopCount = 0;
    while (CURR_SIZE < sizeRequired && loopCount == 0){
        if (CURR->magicNumber != FREE){
            fprintf(stderr, "Error: Header of memory block has been overwritten. Aborting program\n");
            abort();
        }
        CURR = NEXT;
        if (CURR == currentNode){
            loopCount++;
        }
    }
    //Have to repeat this because it won't check if the first memory block in free_list has enough memory
    if (CURR->magicNumber != FREE){
        fprintf(stderr, "Error: Header of memory block has been overwritten. Aborting program\n");
        abort();
    }

    if (loopCount == 0){
        while (CURR_SIZE != sizeRequired){
            partition();
        }
        if (NEXT == CURR && PREV == CURR){
            printf("Must have at least one free region of memory.\n");
            return NULL;
        }
        printf("free_list_ptr = %p\n", CURR);
        byte* alloc = ((byte*)CURR)+HEADER_SIZE;

        markAssigned();
        return alloc;
    } else {
        printf("Not enough memory\n");
        return NULL;
    }
}

void partition(void){
    u_int32_t sizeOfPartition = CURR_SIZE/2;
    Header currentNode = (Header)CURR;
    Header nextNode = (Header)((byte*)CURR + sizeOfPartition);

    currentNode->magicNumber = FREE;
    currentNode->size = sizeOfPartition;

    nextNode->magicNumber = FREE;
    nextNode->size = sizeOfPartition;

    //free_list_ptr = currentNode;

    currentNode->next->prev = nextNode;
    nextNode->prev = currentNode;
    nextNode->next = currentNode->next;
    currentNode->next = nextNode;
}

void markAssigned(void){
    CURR->magicNumber = USED;
    PREV->next = NEXT;
    NEXT->prev = PREV;
    CURR = NEXT;
}


void allocator_free(void *object){
    byte* temp = (byte*)object;
    temp = temp - HEADER_SIZE;
    Header freeAddress = (Header)temp;
    //printf("Trying to free %p\n", freeAddress);

    findLocationOfFree(freeAddress);
    freeAddress->magicNumber = FREE;
    freeAddress->prev = CURR;
    freeAddress->next = NEXT;
    NEXT->prev = freeAddress;
    NEXT = freeAddress;
    //printf("free_list_ptr->next = %p, size = %d\n", NEXT, NEXT_SIZE);
    /*printf("\nTesting merge\n\n");
    int i;
    for(i=0;i<10;i++){
            printf("ptr = %p, ptr->size = %d\n", CURR, CURR_SIZE);
            CURR = NEXT;
        }
        printf("\n");
    */
    int canMerge = merge();

    

    while(canMerge == 1){
        /*for(i=0;i<10;i++){
            printf("ptr = %p, ptr->size = %d\n", CURR, CURR_SIZE);
            CURR = NEXT;
        }
        printf("\n");
        */
        canMerge = merge();
    }
}

void findLocationOfFree(Header freeAddress){
    int loopCount = 0;
    int addressDifference = CURR - freeAddress;
    int minAddressDifference = addressDifference;
    Header minAddress = CURR;
    Header currentNode = CURR;
    while (loopCount == 0){
        CURR = NEXT;
        addressDifference = CURR - freeAddress;
        if (abs(addressDifference) < abs(minAddressDifference)){
            minAddressDifference = addressDifference;
            minAddress = CURR;
        }
        //printf("Address difference = %d\n", addressDifference);
        if (CURR == currentNode){
            loopCount++;
        }
    }
    if(minAddressDifference > 0){
        CURR = minAddress->prev;
    } else {
        CURR = minAddress;
    }
}


int merge(void){
    int canMerge = 0;
    int loopCount = 0;
    Header currentNode = CURR;

    while (canMerge == 0 && loopCount == 0){
    /*     printf("CURR_SIZE = %d\n"
                "NEXT_SIZE = %d\n"
                "address - memory = %d\n"
                "address difference = %d\n\n",
                CURR_SIZE, NEXT_SIZE, 
                ((byte*)CURR - memory),
                HEADER_SIZE*(NEXT - CURR));
    */
        canMerge = checkMerge();
    
        CURR = NEXT;
        if (CURR == currentNode){
            loopCount++;
        }
    }
    //printf("CanMerge = %d\n", canMerge);
    Header temp = CURR;
    if(canMerge == -1){
        CURR_SIZE *= 2;
        temp = temp->next->next;
        temp->next->prev = CURR;
        NEXT = temp; 
    } else if (canMerge == 1){
        CURR = PREV;
        CURR_SIZE *= 2;
        temp->next->prev = CURR;
        NEXT = temp->next; 
    }

    return canMerge;
}

int checkMerge(void){
     if(CURR_SIZE == NEXT_SIZE && (HEADER_SIZE*(NEXT - CURR) == CURR_SIZE)){
         if((((byte*)NEXT + CURR_SIZE) - memory)%(CURR_SIZE*2)){
             return CANNOT_MERGE;
         } else {
             return MERGE_WITH_NEXT_BLOCK;
         }
     } else if (CURR_SIZE == PREV_SIZE && HEADER_SIZE*(CURR - PREV == CURR_SIZE)){

         if((((byte*)CURR + CURR_SIZE) - memory)%(CURR_SIZE*2)){
             return CANNOT_MERGE;
         } else {
             return MERGE_WITH_PREV_BLOCK;
         }
     }
     return CANNOT_MERGE;
    
}

void allocator_end(void){
    printf("Checking if memory exists to free\n");
    assert(memory != NULL);
    printf("Freeing allocated memory\n");
    free(memory);
    memory = NULL;
}
