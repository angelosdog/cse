#include <stdio.h>
#include <assert.h>
#include "Item.h"
#include "adt.h"

void LISTprint();
int main(void) {

  Item item0;
  Item item1;
  Item item2;

  setbuf(stdout, NULL);

  item0 = 10;
  item1 = 11;
  item2 = 12;

  printf("LIST init()\n");
  LISTinit();
  printf("LIST is empty\n");
  assert(LISTempty() == 1);
  printf("passed!\n\n");

  printf("LIST after(10), LIST after(12), LIST before(11)\n");
  LISTafter(item0);
  LISTafter(item2);
  LISTbefore(item1);
  printf("theList: 10 -> [11] -> 12 -> NULL\n");
  printf("LIST current item is 11\n");
  assert(LISTcurrent() == item1);
  printf("passed!\n\n");

  printf("LIST move back 1\n");
  assert(LISTmove(-1) == 1);
  printf("theList: [10] -> 11 -> 12 -> NULL\n");
  printf("LIST current item is 10\n");
  assert(LISTcurrent() == item0);
  printf("passed!\n\n");

  printf("LIST move forward 2\n");
  assert(LISTmove(2) == 1);
  printf("theList: 10 -> 11 -> [12] -> NULL\n");
  printf("LIST current item is 12\n");
  assert(LISTcurrent() == item2);
  printf("passed!\n\n");

  printf("LIST delete last item\n");
  LISTdelete();
  printf("theList: 10 -> [11] -> NULL\n");
  assert(LISTcurrent() == item1);
  printf("passed!\n\n");

  printf("LIST move back 1\n");
  assert(LISTmove(-1) == 1);
  printf("theList: [10] -> 11 -> NULL\n");
  printf("LIST delete first item\n");
  LISTdelete();
  printf("theList: [11] -> NULL\n");
  assert(LISTcurrent() == item1);
  printf("passed!\n\n");

  printf("LIST delete last item\n");
  LISTdelete();
  printf("LIST is empty\n");
  assert(LISTempty() == 1);
  printf("passed!\n\n");

  printf("You are AWESOME! You passed all the adt tests.\n");
  return 0;
}

