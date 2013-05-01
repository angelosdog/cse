// C4Game abstract data type
// 
// Tim Lambert  2012

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "C4Game.h"

typedef struct _c4game {
  int turnNumber;
  char cells[NUM_ROWS][NUM_COLUMNS];
} c4game;

C4Game createGame(void){
  int i;
  int j;
  C4Game new = malloc(sizeof(c4game));
  //(*new).turnNumber = 0; alternate method of referring
  new->turnNumber = 0;
  for (i = 0; i < NUM_ROWS; i++){
    for (j = 0; j < NUM_COLUMNS; j++){
      new->cells[i][j] = EMPTY;
    }
  }
  return new;
}

void disposeGame(C4Game game){
  free(game);
}

void dropCounter(C4Game game, int column){
  int i;
  int row = 0;
  char player;
  for (i = 0; i < NUM_ROWS; i++){
    if (game->cells[i][column] == EMPTY){
      row = i;
    }
  }
  if (game-> turnNumber % 2 == 0){
    player = 'R';
  } else {
    player = 'Y';
  }
  game->cells[row][column] = player;
  game -> turnNumber++;
}

int canDrop(C4Game game, int column){
  return game->cells[0][column] == EMPTY ;
}

void printGame(C4Game game){
  int i;
  int j;
  for (i = 0; i < NUM_ROWS; i++){
    for (j = 0; j < NUM_COLUMNS; j++){
      putchar(game->cells[i][j]);
    }
    putchar('\n');
  }
}

char getCell(C4Game game, int row, int column){
  char answer = EMPTY;
  //all cells off the board are empty
  if (0 <= row && row <NUM_ROWS &&
      0 <= column && column <NUM_COLUMNS){
    answer = game->cells[row][column];
  }
  return answer;
}
