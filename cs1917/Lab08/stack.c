#include <stdlib.h>
#include "stack.h"
#define MAX_STACK 100

typedef struct _stack{
    int contents[MAX_STACK];
    int topPointer;
}stack;

int top(Stack s){
    return s->contents[s->topPointer];
}

void pop(Stack s){
    s->topPointer--;
}

void push(Stack s, int value){
    s->topPointer++;
    s->contents[s->topPointer] = value;
}

Stack createStack(void){
    Stack s;
    s = malloc(sizeof(stack));
    s->topPointer = -1;
    return s;
}

// 0 if stack is empty
// 1 if stack is not empty
int isEmpty(Stack s){
    int empty;
    if(s->topPointer == -1){
        empty = 0;
    } else {
        empty = 1;
    }
    return empty;
}

void disposeStack(Stack s){
    free(s);
}
