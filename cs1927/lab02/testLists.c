#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "lists.h"

void printListReverse(dlink list);
void printDList(dlink list);

// insert proper tests here
int main (int argc, const char * argv[]) {
  link oneToTen = fromTo (1, 10);
  printList (oneToTen);
  printf("%d\n", sumListItems(oneToTen));
  link fourToThree = fromTo(4, 3);
  printList(fourToThree);
  link fiveToFive = fromTo(5, 5);
  printList(fiveToFive);
  dlink dOneToTen = doublify(oneToTen);
  printDList(dOneToTen);
  printListReverse(dOneToTen);
  dlink dFourToThree = doublify(fourToThree);
  printListReverse(dFourToThree);
  dlink dFiveToFive = doublify(fiveToFive);
  printListReverse(dFiveToFive);
  return 0;
}

void printDList(dlink list){
  dlink curr = list;
  while (curr != NULL){
    printf("[%d]->", curr->item);
    curr = curr->next;
  }
  printf("[X]\n");
}

void printListReverse(dlink list){
  printf("[X]");
  if (list != NULL){
    while (list->next != NULL){
      list = list->next;
    }
    while (list != NULL){
      printf("->[%d]", list->item);
      list = list->prev;
    }
  }
  printf("\n");
}
