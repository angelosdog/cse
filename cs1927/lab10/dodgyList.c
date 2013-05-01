#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "bool.h"
#include "item.h"
struct _list{
    Item item;
    List next;
};

//Creates a new list to be used
// @return, an empty list
List newList() {
    return NULL;
}
//Adds an item to the list
//If l is NULL returns a new list containting the item I
//Should be used in place of a create List function.
//  @param i, item to be added to the list
//  @param l, the list to be added to
//  @return the list with the item added
List add(Item i, List l) {
    List temp = l;
    l = malloc(sizeof(struct _list));
    l->item = i;
    l->next = temp;
    return l;
}

//checks if the list contains an item
//  @param i, the item to check for
//  @param l, the list to check in
//true if the list contains the item, else false
bool listContains(Item i, List l) {
    while(l != NULL && compare(l->item, i) == (false)){
        l = l->next;
    }
    if (l == NULL) return false;
    return true;
}

//deletes the list
//  @param l, the list to be deleted 
//  @retun NULL
List deleteList(List l) {
    List temp = l;
    while(l != NULL){
        l = l->next;
        free(temp);
        temp = l;
    }
    return NULL;
}
