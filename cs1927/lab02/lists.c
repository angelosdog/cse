#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "lists.h"

link fromToLoop(int start, int end);

void printList (link list) {
  link curr = list;
  while (curr != NULL) {
    printf ("[%d]->", curr->item);
    curr = curr->next;
  }
  printf ("[X]\n");
}


int sumListItems(link ls){
	int sum = 0;
	while (ls != NULL){
  	sum+=ls->item;
		ls = ls->next; 
	}
	return sum;
}

link fromToLoop(int start, int end){
	link first = (link) malloc (sizeof *first);
	link curr, prev;
	if (end < start){
		first = NULL;
  } else {
		first->item = start;
		first->next = NULL;
		start++;
		prev = first;
		while (start <= end){
			curr = (link)malloc(sizeof *curr);
			curr->item = start;
			curr->next = NULL;
			prev->next = curr;
			prev = curr;
			start++;
		}
	}
	return first;
}

link fromTo(int start, int end) {
	link first = (link) malloc(sizeof *first);
	if (start > end){
    first = NULL;
  } else {
		first->item = start;
		first->next = fromTo(start+1, end);
	}
	return first;
}


dlink doublify(link ls){
	dlink first = (dlink) malloc(sizeof *first);
 	dlink curr;
 	dlink previous = first;
  if (ls != NULL){
  	first->prev = NULL;
  	first->item = ls->item;
  	first->next = NULL;
  	while (ls->next != NULL){
  		ls = ls->next;
  		curr = (dlink) malloc(sizeof *curr);
      previous->next = curr;
  		curr->prev = previous;
  		curr->item = ls->item;
  		curr->next = NULL;
      previous = curr;
  	}
  } else {
    first = NULL;
  }
  return first;
}

