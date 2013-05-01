#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "stack.h"

#define MAX_STRING_SIZE 200
#define TRUE 2
#define FALSE 0
int main(int argc, char* argv[]){

    Stack s;
    s = createStack();
    int input = getchar();
    int isBalanced = 1;
    
    while((input != EOF || isEmpty(s) == 1) && isBalanced == 1) {
        if(input == '(' || input == '{' || input == '['){
            //printf("%c pushed onto stack\n", input);
            push(s, input);
        } else if(top(s) == '(' && input == ')'){
            //printf("%c popped off of the stack\n", input);
            pop(s);
        } else if(top(s) == '{' && input == '}'){
            // printf("%c popped off of the stack\n", input);
            pop(s);
        } else if(top(s) == '[' && input == ']'){
            //printf("%c popped off of the stack\n", input);
            pop(s);
	} else if (input != ']' && input != '}' && input != ')' && isprint(input)) {
            isBalanced = TRUE;
        } else {
            //printf("Detecting imbalance\n");
            isBalanced = FALSE; 
        }
        input = getchar();
    }

    if (isBalanced == FALSE){
        printf("Unbalanced\n");
    } else {
        printf("Balanced\n");
    }
    
    disposeStack(s);
    return 0;
}
