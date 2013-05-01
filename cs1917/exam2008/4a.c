#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char* argc[]){
    int numChars = 1; //We always have EOF
    int numLines = 0;
    int numWords = 0;
    int input = getChar();
    int newLine = 1;
    while (input != EOF){
        newLine = 1;
        numChars++;
        if (input == ' '){
            numWords++;
        }
        if (input == '\n'){
            numLines++;
            newLine = 0;
        }
        input = getChar();
    }
    printf("Number of characters = %d\n", numChars);
    if(numWords != 0){
        numWords++;
    }
    printf("Number of words = %d\n", numWords);
    if (newLine == 1){
        numLines++;
    }
    printf("Number of lines = %d\n", numLines);
    return 0;
}
