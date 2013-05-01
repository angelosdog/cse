#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"
#define EMPTY_STACK -1
#define TRUE 1
#define FALSE 0

typedef struct _node *nodePointer;
struct _node {
    int value;
    nodePointer next;
};

struct _stack {
    nodePointer first;
};

Stack createStack(void){
    Stack s;
    s = malloc(sizeof(struct _stack));
    s->first = NULL;
    return s;
}

int top(Stack s){
    nodePointer temp = s->first;
    int returnValue;
    if (s->first == NULL){
        //printf("Stack underflow\n");
        returnValue = EMPTY_STACK;
    } else {
        returnValue = temp->value;
    }
    return returnValue;    
} 

int isEmpty(Stack s){
    int empty;
    if (s->first == NULL){
        empty = FALSE;
    } else {
        empty = TRUE;
    }
    return empty;
}

void push(Stack s, int i){
    nodePointer newNode = malloc(sizeof(struct _node));
   
    newNode->value = i;
    newNode->next = s->first;
    s->first = newNode;
}

void pop(Stack s){
    nodePointer temp = s->first;
    s->first = temp->next;
    free(temp);
}

void disposeStack (Stack s){
    nodePointer temp = s->first;
    nodePointer temp2;
    while (temp != NULL){
        temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
    free(s);
}

/* We can't use this for a stack
void appendToList(Stack s, int value){
    nodePointer newNode = malloc(sizeof(struct _node));

    newNode->value = value;
    newNode->next = NULL;
    nodePointer temp = s->first;
    if(temp == NULL){
        s->first = newNode;
    } else {
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newNode;
    }
}
*/

