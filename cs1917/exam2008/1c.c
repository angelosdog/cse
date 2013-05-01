#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int numChars = 0;
    int i;
    char characters[500];
    int input = getchar()
    while (input != EOF && i < 500){
        characters[i] = input;
        numChars++;
        input = getchar();
    }
    for (i = numChars; i >= 0; i--){
        printf("%c", characters[i]);
    }
    return 0;
}
