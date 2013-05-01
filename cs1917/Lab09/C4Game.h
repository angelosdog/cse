// C4Game.h header file for Connect 4 game

#define PLAYER1 'R'
#define PLAYER2 'Y'
#define EMPTY ' '
#define TRUE 1

#define NUM_COLUMNS 7
#define NUM_ROWS 6

typedef struct _c4game *C4Game;

// create a new game
C4Game createGame(void);

//dispose of a game
void disposeGame(C4Game game);

//drop a counter in column
void dropCounter(C4Game game, int column);

//check if a counter can be dropped in a column
int canDrop(C4Game game, int column);

//print the game
void printGame(C4Game game);

//get the contents of a cell in the specified row an column
char getCell(C4Game game, int row, int column);
