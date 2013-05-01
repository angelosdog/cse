#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "item.h"

int hash(Item a) {
    if (a == NULL) return 0;
    int i = 0;
    int hashValue = 0;
    while(a[i] != '\0'){
        hashValue += a[i];
        i++;
    }
    printf("Hash value = %d\n", hashValue);
    return hashValue;
}

bool compare(Item a, Item b) {
    if (strcmp(a, b) == 0) return true;
    return false;
}

