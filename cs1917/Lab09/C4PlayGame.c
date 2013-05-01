// play connect 4
// 
// Tim Lambert  2012

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "C4Game.h"
int main(int argc, char* argv[]){
  C4Game game;
  int column;
  int isWinner = 0;
  game = createGame();

  printf("Which column?\n");
  scanf("%d",&column);
  while (column<NUM_COLUMNS && column >= 0 && isWinner == 0) {
    if (canDrop(game, column)) {
      dropCounter(game, column);
    }
    printGame(game);
    isWinner; // update
    printf("Which column?\n");
    scanf("%d",&column);
  }
  disposeGame(game);

  return EXIT_SUCCESS;
}

