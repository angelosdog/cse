#include <stdio.h>
#include <stdlib.h>
// YOU MAY ALTER THE VALUE OF THE FOLLOWING CONSTANT BUT NOT ITS NAME
#define MAXNUM 10
int main (int argc, char *argv[]) {
    printf ("Grid size: %d\n", MAXNUM);
    int i;
    int j;
    for (i = 0; i <= MAXNUM; i++){
        for(j = 0; j < MAXNUM; j++){
            printf("+-");
        }
        printf("+\n");
    }
    return EXIT_SUCCESS;
}

