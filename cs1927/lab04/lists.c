#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Item.h"
#include "lists.h"


struct _node {
  Item item;
  link next;
} node;

link newNode (Item);
void freeNode (link);


void insertNext (link, link);
link insertHead (link, link);
link deleteNext (link);

link getNext (link); 
Item getItem (link);
int isEmptyList (link);

Item getItem (link);
void printItem (Item);

void insertNode(link ls, Item item){
  link newN = newNode (item);
  assert (ls != NULL); 
  newN->next = ls->next;
  ls->next = newN;
}

link insertHead (link ls, link newNode) {
  assert (newNode);
  newNode->next = ls;
  return newNode;
}


link newNode (Item item) {
  link ls;
  ls = malloc (sizeof (*ls));
  ls->item = item;
  ls->next = NULL;
  return ls;     
}

void freeNode (link l) {
  assert (l);
  free (l);
}

Item getItem (link l) {
  assert (l);
  return (l->item);
}


link deleteNext (link ls) {
  assert (ls != NULL);
  assert (ls->next != NULL);
  link result = ls->next;
  ls->next = ls->next->next;
  return result;
}


void insertNext (link ls, link newN) {
  assert (ls != NULL); 
  newN->next = ls->next;
  ls->next = newN;
}


link getNext (link ls) {
  assert (ls);
  return (ls->next);
}  


int isEmptyList (link ls) {
  return (ls == NULL);
}

void printItem (Item item) {
  printf ("%d",item);
}

void printList (link ls) {
  link curr = ls;
  while (! isEmptyList (curr)) {
    printf ("->[");
    printItem (getItem (curr));
    printf ("]");
    curr = getNext (curr);
  }
  printf ("->[X]\n");
}

int length (link ls) {
  if (isEmptyList (ls)) {
    return 0;
  }
  return (1 + length (ls->next));
}

int length2 (link ls, int prevLength) {
  if (isEmptyList (ls)) {
    return prevLength;
  }
  return (length2 (ls->next, prevLength));
}

void printNode (link ls) {
  printf (" ");
  printItem (ls->item);
  printf (" ");
  return;
}

void traverse (link ls, void (*visit)(link)) {
  if (ls == NULL) {
    return;
  }
  (*visit) (ls);
  traverse (ls->next, visit);
  return;
}

void traverseR (link ls, void (*visit)(link)) {
  if (ls == NULL) {
    return;
  }
  traverseR (ls->next, visit);
  (*visit)(ls);
  return;
}

void square(link l){
  l->item = l->item*l->item;
}

void squareAll(link ls){
  traverse(ls, square);
}

int count(link ls, int (*pred)(Item)){
  int counter = 0;
  if (ls == NULL){
    return 0;
  } else if (pred(ls->item)) {
      counter++;
    }
  return counter + count(ls->next, pred);
}

int isEven (Item n){
  return n%2 == 0;
}

int isOdd(Item n){
  return n%2 == 1;
}

int isZero(Item n){
  return n == 0;
}

int isGreaterThan3(Item n){
  return n > 3;
}

int isLessThan10(Item n){
  return n < 10;
}


/*
int allEven(int a[], int size){
  if (size == 0){
    return TRUE;
  }
  if (size == 1){
    return a[0] % 2 == 0;
  }
  if(allEven(a, size/2)){
    return allEven(&a[size/2], size - size/2);
  }
  return FALSE;
}
*/
