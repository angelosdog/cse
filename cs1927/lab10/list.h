#ifndef LIST 
#define LIST 1==1
#include "bool.h"
#include "item.h"
typedef struct _list * List;

//Creates a new list to be used
// @return, an empty list
List newList();

//Adds an item to the list
//If l is NULL returns a new list containting the item I
//Should be used in place of a create List function.
//  @param i, item to be added to the list
//  @param l, the list to be added to
//  @return the list with the item added
List add(Item i, List l);

//checks if the list contains an item
//  @param i, the item to check for
//  @param l, the list to check in
//true if the list contains the item, else false
bool listContains(Item i, List l);

//deletes the list
//  @param l, the list to be deleted 
//  @retun NULL
List deleteList(List l);

#endif
