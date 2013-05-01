#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "adt.h"


typedef struct _dnode *dlink;

typedef struct _dnode {
 Item item;
 dlink prev;
 dlink next; 
} dnode;

typedef struct _dlist * dlist;

typedef struct _dlist{
  dlink first;
  dlink last;
  dlink curr;
  int position;
  int size;
} dheader;

dlist header;

void LISTinit(void){
  header = (dlist) malloc(sizeof(*header));
  header->first = NULL;
  header->last = NULL;
  header->curr = NULL;
  header->position = 0;
  header->size = 0;
}

int LISTempty(void){
  return (header->first == NULL);
}

int LISTmove(int n){
//printf("curr->item = %d\n", header->curr->next->item);
  if (header->position + n < 0 || header->position + n >= header->size){
    printf("Error: Moved out of list bounds");
    abort();
  }
  int finalPosition = header->position+n;
//printf("curr->item = %d\n", header->curr->next->item);
  if(n < 0){
    while (finalPosition+1 <= header->position){
      header->curr = header->curr->prev;
      header->position--;
    }
  } else if (n > 0){
    while (finalPosition > header->position){
      header->curr = header->curr->next;
      header->position++;
    }
  }
//printf("curr->item = %d\n", header->curr->next->item);
  int returnVal;
  if (header->curr == header->first || header->curr == header->last){
    returnVal = 1;
  } else{
    returnVal = 0;
  }
//printf("curr->item = %d\n", header->curr->next->item);
  return returnVal;
}

Item LISTcurrent(void){
  return header->curr->item;
}

void LISTbefore (Item newItem){
  dlink before = (dlink)malloc(sizeof(*before));
  before->item = newItem;
  if (header->size != 0){
    before->next = header->curr;
    before->prev = header->curr->prev; 
    if (header->curr == header->first){
     header->first = before;
    } else {
      header->curr->prev->next = before;
    }
    header->curr->prev = before;
    header->curr = before;
  } else {
    before->next = NULL;
    before->prev = NULL;
    header->curr = before;
    header->first = before;
    header->last = before;
  }
  header->size++;
}

void LISTafter (Item newItem){
  dlink after = (dlink) malloc(sizeof(*after));
  after->item = newItem;

  if(header->size != 0){
    after->next = header->curr->next;
    after->prev = header->curr;
    if (header->curr == header->last){
      header->last = after;
    }
    header->curr->next = after;
    header->curr = after;
    header->position++;
  } else {
    after->next = NULL;
    after->prev = NULL;
    header->curr = after;
    header->last = after;
    header->first = after;
  }
  header->size++;
}

Item LISTdelete (void){
  Item itemDeleted;
  dlink temp;
  if (header->curr == header->first){
    temp = header->curr;
    itemDeleted = temp->item;
    header->curr = header->curr->next;
    header->first = header->curr;
    free(temp);
  } else {
    temp = header->curr;
    itemDeleted = temp->item;
    temp->prev->next = temp->next;
    if(temp->next != NULL){
      temp->next->prev = temp->prev;
    }
    header->curr = header->curr->prev;
    free(temp);
  }
  header->size--;
  return itemDeleted;
}
