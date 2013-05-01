#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashTable.h"

#define MAX_DICT_SIZE       350000
#define MAX_STRING_LENGTH   59    //Note longest word to be seen in dictionary is 
//	Llanfairpwllgwyngyllgogerychwyrndrobwllllantysiliogogogoch
int main () {

    //Create the hash table we are going to use 
    HashTable h = newHashTable();

    //Open up a dictionary of words to use from the start
    FILE * input = fopen("dictionary.txt", "r");
    if (input == NULL) return EXIT_FAILURE;

    //Malloc some space for the strings we are reading in from the dictionary
    char ** temp = malloc(sizeof(char*) * MAX_DICT_SIZE);
    int k = 0;
    for (k = 0; k < MAX_DICT_SIZE; k++) {
        temp[k] = malloc(sizeof(char) * MAX_STRING_LENGTH);
    }

    //Read in everything from the dictionary
    int i = 1;
    int j = 0;
    temp[j][0] = fgetc(input);
    char stream = temp[j][0];
    while (temp[j][i-1] && stream != EOF) {
        temp[j][i] = fgetc(input);
        stream = temp[j][i];
        if (temp[j][i] == '\n') {
            temp[j][i] = '\0';
            if (hashTableContains(temp[j], h)) {
                printf("\nSeen %s\n", temp[j]);
            } else {
                printf("\nNot seen %s\n", temp[j]);
                printf("Adding: %s\n", temp[j]);
                put(temp[j], h);
            }
            assert(hashTableContains(temp[j], h));
            i = 0;
            j++;
            temp[j][0] = fgetc(input);
            stream = temp[j][0];
        }
        i++;
    }
    //close the dictionary
    fclose(input);
    printf("%d\n", i);
    i = 1;
    //Allow for user input
    temp[j][0] = fgetc(stdin);
    stream = temp[j][0];
    while (temp[j][i-1] && stream != EOF) {
        temp[j][i] = fgetc(stdin);
        stream = temp[j][i];
        if (temp[j][i] == '\n') {
            temp[j][i] = '\0';
            if (hashTableContains(temp[j], h)) {
                printf("\nSeen %s\n", temp[j]);
            } else {
                printf("\nNot seen %s\n", temp[j]);
                printf("Adding: %s\n", temp[j]);
                put(temp[j], h);
            }
            assert(hashTableContains(temp[j], h));
            i = 0;
            j++;
            temp[j][0] = fgetc(stdin);
            stream = temp[j][0];
        }
        i++;
    }
    //free all the strings we have malloc'ed
    for (k=0; k < MAX_DICT_SIZE; k++) {
        free(temp[k]);
    }
    free(temp);

    //delete the hash table we used
    h = deleteHashTable(h);

    //exit
    return EXIT_SUCCESS;
}
