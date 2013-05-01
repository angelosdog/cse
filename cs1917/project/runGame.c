// runGame.c v 0.999999 - the inefficient and repetitive edition
// Vincent Tran (mostly)
// Tristram Healy
// Apr 2012

/* Edited and Git'ed by Tristram Healy */
/* I have no idea if this code works at all */
/* TODO Notes will be in multiline comments like this. [Tristram] */

#include <stdio.h>
#include <stdlib.h>

/* For seeding rand */
#include <time.h>

#include "Game.h"
#include "ai.h"
//#include "testGame.h"

#define NO_ACTION -1
#define WIN_SCORE 150
#define WIDTH_OF_BOARD 115
#define HEIGHT_OF_BOARD 32

#define RED_BACKGROUND     "\x1b[41m"
#define GREEN_BACKGROUND   "\x1b[42m"
#define YELLOW_BACKGROUND  "\x1b[43m"
#define BLUE_BACKGROUND    "\x1b[44m"
#define MAGENTA_BACKGROUND "\x1b[45m"
#define CYAN_BACKGROUND    "\x1b[46m"
#define DEFAULT  "\x1B[0m"

#define RED_TEXT     "\x1b[31m"
#define GREEN_TEXT   "\x1b[32m"
#define YELLOW_TEXT  "\x1b[33m"
#define BLUE_TEXT    "\x1b[34m"
#define MAGENTA_TEXT "\x1b[35m"
#define CYAN_TEXT    "\x1b[36m"

/* 
 * These structs definitely need a new home, most likely Game.c when that's written.
 * Also I think we could cut down the amount of structs and use more arrays.
 * (these are TODO notes btw)
 */

// This funtion is to print out the game board

// This function is used to determine what colour to paint each individual hexagon based off the discipline associated with that region
typedef struct _board* Board;
typedef struct _board{
    char gameBoard [HEIGHT_OF_BOARD][WIDTH_OF_BOARD];
} board;
static Board createBoard(void);
static void printBoard(Game g, Board b);
static int findCentreX(int regionID);
static int findCentreY(int regionID);
static void drawRegion(Game g, Board b, int x, int y, int regionID);
static void drawTable(Game g, Board b);
static void colourMap(Game g, Board b, int x, int y);
static void colourTable(Board b, int x, int y);
static char* disciplineOfRegion (int regionID, Game g);

int main(int argc, char* argv[]){

    Game g;
    int disciplines[NUM_REGIONS] = {5, 3, 3, 2, 4, 3, 1, 1, 4, 0, 3, 1, 5, 3, 5, 4, 2, 2, 4}; 
    int diceValues[NUM_REGIONS] = {6, 10, 8, 3, 4, 9, 6, 11, 11, 7, 12, 3, 5, 2, 5, 10, 9, 4, 8};
    g = newGame(disciplines, diceValues);
    Board b;
    b = createBoard();

    /* Quick and nasty rand() seeding */
    srand(time(NULL));
    int diceScore;
    int legalAction;
    action a;
    // g.currentTurn should initialise to -1
    int kpiA = getKPIpoints(g, UNI_A);
    int kpiB = getKPIpoints(g, UNI_B);
    int kpiC = getKPIpoints(g, UNI_C);
    int player = 0; 

    while (kpiA < WIN_SCORE && kpiB < WIN_SCORE && kpiC < WIN_SCORE) {
        // The loop next loop braks when the a.actionCode == 0 
        //so we need to reset it
        a.actionCode = NO_ACTION;
        /* rand() will return a pseduo random number between 0 and RAND_MAX
         * use %6 to get something between 0 to 5
         */

        diceScore = rand()%6 + rand()%6 + 2; // [FIXED] I'm not sure what the random function is
        	
        throwDice(g, diceScore);

        player = getWhoseTurn(g); //turnNumber%3+1
        while (a.actionCode != PASS){
            // reset the values of legalAction and a.actionCode so 
            // that the loop at the end will repeat
            // until the actionCode is actually PASS
	    legalAction = FALSE;
            a.actionCode = NO_ACTION;
            a.destination[0] = '\0';
            a.disciplineFrom = NO_ACTION;
            a.disciplineTo = NO_ACTION;
            printBoard(g, b);

            printf("Enter an action code\n"
                   "Pass:             0\n"
                   "Build Campus:     1\n"
                   "Build GO8:        2\n"
                   "Obtain ARC Grant: 3\n"
                   "Start Spinoff:    4\n"
                   "Retrain Students: 7\n");
            while (legalAction == FALSE){
                if (player == 1){
                    printf("UNI_A > ");
                } else if (player == 2){
                    printf("UNI_B > ");
                } else {
                    printf("UNI_C > ");
                }                
                scanf("%d", &a.actionCode);

                // Check for valid action code
                if (!((a.actionCode >= PASS && a.actionCode <= START_SPINOFF) || a.actionCode == RETRAIN_STUDENTS)){
                    printf("Incorrect input. Please enter another action.\n");
                } else if (isLegalAction(g, a) == FALSE){
                    printf("Not a legal action. Enter another action.\n");
                } else {
                    legalAction = TRUE;
                    if (a.actionCode == RETRAIN_STUDENTS){
                        printf("Enter what kind of student you want to retrain, followed by what they will retrain into\n"
                               "THD:    0\n"
                               "BPs:    1\n"
                               "BQN:    2\n"
                               "MJ:     3\n"
                               "MTV:    4\n"
                               "MMONEY: 5\n");
                        scanf("%d %d", &a.disciplineFrom, &a.disciplineTo);
                    }
                    makeAction(g, a);
                }
            }

        }
        kpiA = getKPIpoints(g, UNI_A);
        kpiB = getKPIpoints(g, UNI_B);
        kpiC = getKPIpoints(g, UNI_C);

    }
    disposeGame(g);
    return EXIT_SUCCESS;
}

static Board createBoard(void){
    int x;
    int y;
    Board b;
    b = malloc(sizeof(board));
    // initialise all nodes in the board as whitespace
    for(y = 0;y < HEIGHT_OF_BOARD;y++){
        for(x = 0;x < WIDTH_OF_BOARD;x++){
            b->gameBoard[y][x] = ' ';
        }
    }
    return b;
}

static void printBoard (Game g, Board b){
    /* The board is made up of 19 hexagons, so we will print
    out the 19 hexagons individually. In doing so, we may end up
    overwriting the same space twice, but the contents of that
    space will be the same, so it is fine to do so.
    
    Use the following print out as a reference for co-ordinates
    of the centres of the hexagons as well as the region ID of
    the hexagon

                        #----#
                       /      \
                      /        \
                #----#     7    #----#
               /      \        /      \
              /        \      /        \
        #----#     3    #----#    12    #----#
       /      \        /      \        /      \
      /        \      /        \      /        \
     #     0    #----#     8    #----#    16    #
      \        /      \        /      \        /
       \      /        \      /        \      /
        #----#     4    #----#    13    #----#
       /      \        /      \        /      \
      /        \      /        \      /        \
     #     1    #----#     9    #----#    17    #
      \        /      \        /      \        /
       \      /        \      /        \      /
        #----#     5    #----#    14    #----#
       /      \        /      \        /      \
      /        \      /        \      /        \
     #     2    #----#    10    #----#    18    #
      \        /      \        /      \        /
       \      /        \      /        \      /
        #----#     6    #----#    15    #----#
              \        /      \        /
               \      /        \      /
                #----#    11    #----#
                      \        /
                       \      /
                        #----#
*/   

   int regionID;
   int centreX;
   int centreY;
   for (regionID = 0; regionID < NUM_REGIONS; regionID++){
       centreX = findCentreX(regionID);
       centreY = findCentreY(regionID);
       drawRegion(g, b, centreX, centreY, regionID);    
   }
   drawTable(g, b);
   int i;
   int j;

   for (i = 0; i < HEIGHT_OF_BOARD; i++){
       for (j = 0; j < WIDTH_OF_BOARD; j++){
           if (j < 50){
               colourMap(g, b, j, i);
           } else {
               colourTable(b, j, i);
           }
           printf("%c", b->gameBoard[i][j]);
       }
       printf("\n");
   } 
}

static int findCentreY(int regionID){
    int centreY;
    if (regionID == 7){
        centreY = 3;
    } else if (regionID == 3 || regionID == 12){
        centreY = 6;
    } else if (regionID == 0 || regionID == 8 || regionID == 16){
        centreY = 9;
    } else if (regionID == 4 || regionID == 13){
        centreY = 12;
    } else if (regionID == 1 || regionID == 9 || regionID == 17){
        centreY = 15;
    } else if (regionID == 5 || regionID == 14){
        centreY = 18;
    } else if (regionID == 2 || regionID == 10 || regionID == 18){
        centreY = 21;
    } else if (regionID == 6 || regionID == 15){
        centreY = 24;
    } else {
        centreY = 27;
    }
    return centreY;
}

static int findCentreX(int regionID){
    int centreX;
    if (regionID >= 0 && regionID < 3){
        centreX = 6;
    } else if (regionID >= 3 && regionID < 7){
        centreX = 14;
    } else if (regionID >= 7 && regionID < 12){
        centreX = 22;
    } else if (regionID >= 12 && regionID < 16){
        centreX = 30;
    } else {
        centreX = 38;
    }
    return centreX;
}

static void drawRegion(Game g, Board b, int y, int x, int regionID){

    // break a hexagon into 13 parts; its 6 edges, its 6 vertices
    // and the centre
    // start from the centre, then the rightmost vertex, and then work clockwise
    char temp[2];
    sprintf(temp, "%d", getDiceValue(g, regionID));

    if (temp[0] != '\0'){
        b->gameBoard[y][x] = temp[0];
    }
    if (temp[1] != '\0'){
        b->gameBoard[y][x+1] = temp[1];
    }   
    b->gameBoard[y][x+6] = '#';

    b->gameBoard[y+1][x+5] = '/';
    b->gameBoard[y+2][x+4] = '/';

    b->gameBoard[y+3][x+3] = '#';

    b->gameBoard[y+3][x+2] = '-';
    b->gameBoard[y+3][x+1] = '-';
    b->gameBoard[y+3][x] = '-';
    b->gameBoard[y+3][x-1] = '-';
 
    b->gameBoard[y+3][x-2] = '#';

    b->gameBoard[y+2][x-3] = '\\';
    b->gameBoard[y+1][x-4] = '\\';

    b->gameBoard[y][x-5] = '#';
    
    b->gameBoard[y-1][x-4] = '/';
    b->gameBoard[y-2][x-3] = '/';
 
    b->gameBoard[y-3][x-2] = '#';
 
    b->gameBoard[y-3][x-1] = '-';
    b->gameBoard[y-3][x] = '-';
    b->gameBoard[y-3][x+1] = '-';
    b->gameBoard[y-3][x+2] = '-';

    b->gameBoard[y-3][x+3] = '#';

    b->gameBoard[y-2][x+4] = '\\';
    b->gameBoard[y-1][x+5] = '\\';
}

static void colourMap(Game g, Board b, int x, int y){
    // In case the node we are checking is not in the hexagon
    printf(DEFAULT);
    // Find the centres of each hexagon, iterate around that hexagon and change colour accordingly
    int centreX;
    int centreY;
    int regionID;
    int row;
    int column;
    char* colour;
    for (regionID = 0; regionID < NUM_REGIONS; regionID++){
        centreX = findCentreX(regionID);
        centreY = findCentreY(regionID);
        for (row = centreX-2; row < centreX+4; row++){
            column = centreY+2;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, g);
                printf(colour);
            }
        }
        for (row = centreX-3; row < centreX+5; row++){
            column = centreY+1;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, g);
                printf(colour);
            }
        }
        for (row = centreX-4; row < centreX+6; row++){
            column = centreY;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, g);
                printf(colour);
            }
        }
        for (row = centreX-3; row < centreX+5; row++){
            column = centreY-1;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, g);
                printf(colour);
            }
        }
        for (row = centreX-2; row < centreX+4; row++){
            column = centreY-2;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, g);
                printf(colour);
            }
        }
 
    }
   
}

static char* disciplineOfRegion (int regionID, Game g){
    char* colour = DEFAULT;
    int discipline;
    discipline = getDiscipline(g, regionID); 
    if (discipline == STUDENT_THD){
        colour = BLUE_BACKGROUND;
    } else if (discipline == STUDENT_BPS) {
        colour = RED_BACKGROUND;
    } else if (discipline == STUDENT_BQN) {
        colour = CYAN_BACKGROUND;
    } else if (discipline == STUDENT_MJ) {
        colour = YELLOW_BACKGROUND;
    } else if (discipline == STUDENT_MTV) {
        colour = GREEN_BACKGROUND;
    } else if (discipline == STUDENT_MMONEY) {
        colour = MAGENTA_BACKGROUND;
    }
    
    return colour;
}

static void drawTable(Game g, Board b){

    sprintf(&(b->gameBoard[0][50]),  "|-------------------|-------------------|-------------------|");
    sprintf(&(b->gameBoard[1][50]),  "| UNI_A             | UNI_B             | UNI_C             |");
    sprintf(&(b->gameBoard[2][50]),  "|-------------------|-------------------|-------------------|");
    sprintf(&(b->gameBoard[3][50]),  "| KPI:          %3d | KPI:          %3d | KPI:          %3d |", getKPIpoints(g, UNI_A), getKPIpoints(g, UNI_B), getKPIpoints(g, UNI_C));
    sprintf(&(b->gameBoard[4][50]),  "| Campuses:     %3d | Campuses:     %3d | Campuses:     %3d |", getCampuses(g, UNI_A), getCampuses(g, UNI_B), getCampuses(g, UNI_C));
    sprintf(&(b->gameBoard[5][50]),  "| GO8s:         %3d | GO8s:         %3d | GO8s:         %3d |", getGO8s(g, UNI_A), getGO8s(g, UNI_B), getGO8s(g, UNI_C));
    sprintf(&(b->gameBoard[6][50]),  "| ARC Grants:   %3d | ARC Grants:   %3d | ARC Grants:   %3d |", getARCs(g, UNI_A), getARCs(g, UNI_B), getARCs(g, UNI_C) );
    sprintf(&(b->gameBoard[7][50]),  "| IPs:          %3d | IPs:          %3d | IPs:          %3d |", getIPs(g, UNI_A), getIPs(g, UNI_B), getIPs(g, UNI_C));
    sprintf(&(b->gameBoard[8][50]),  "| Publications: %3d | Publications: %3d | Publications: %3d |", getPublications(g, UNI_A), getPublications(g, UNI_B), getPublications(g, UNI_C));
    sprintf(&(b->gameBoard[9][50]),  "| THDs:         %3d | THDs:         %3d | THDs:         %3d |", getStudents(g, UNI_A, STUDENT_THD), getStudents(g, UNI_B, STUDENT_THD), getStudents(g, UNI_C, STUDENT_THD));
    sprintf(&(b->gameBoard[10][50]), "| BPs:          %3d | BPs:          %3d | BPs:          %3d |", getStudents(g, UNI_A, STUDENT_BPS), getStudents(g, UNI_B, STUDENT_BPS), getStudents(g, UNI_C, STUDENT_BPS));
    sprintf(&(b->gameBoard[11][50]), "| BQNs:         %3d | BQNs:         %3d | BQNs:         %3d |", getStudents(g, UNI_A, STUDENT_BQN), getStudents(g, UNI_B, STUDENT_BQN), getStudents(g, UNI_C, STUDENT_BQN));
    sprintf(&(b->gameBoard[12][50]), "| MJs:          %3d | MJs:          %3d | MJs:          %3d |", getStudents(g, UNI_A, STUDENT_MJ), getStudents(g, UNI_B, STUDENT_MJ), getStudents(g, UNI_C, STUDENT_MJ));
    sprintf(&(b->gameBoard[13][50]), "| MTVs:         %3d | MTVs:         %3d | MTVs:         %3d |", getStudents(g, UNI_A, STUDENT_MTV), getStudents(g, UNI_B, STUDENT_MTV), getStudents(g, UNI_C, STUDENT_MTV));
    sprintf(&(b->gameBoard[14][50]), "| MMONEY:       %3d | MMONEY:       %3d | MMONEY:       %3d |", getStudents(g, UNI_A, STUDENT_MMONEY), getStudents(g, UNI_B, STUDENT_MMONEY), getStudents(g, UNI_C, STUDENT_MMONEY));
    sprintf(&(b->gameBoard[15][50]), "|-------------------|-------------------|-------------------|"); 

}

static void colourTable(Board b, int x, int y){
    printf(DEFAULT);
    if (y == 9 && ((x >= 52 && x <= 68)|| (x >= 72 && x <= 88) || (x >= 92 && x <= 108))){
        printf(BLUE_TEXT);
    } else if (y == 10 && ((x >= 52 && x <= 68)|| (x >= 72 && x <= 88) || (x >= 92 && x <= 108))){
        printf(RED_TEXT);
    } else if (y == 11 && ((x >= 52 && x <= 68)|| (x >= 72 && x <= 88) || (x >= 92 && x <= 108))){
        printf(CYAN_TEXT);
    } else if (y == 12 && ((x >= 52 && x <= 68)|| (x >= 72 && x <= 88) || (x >= 92 && x <= 108))){
        printf(YELLOW_TEXT);
    } else if (y == 13 && ((x >= 52 && x <= 68)|| (x >= 72 && x <= 88) || (x >= 92 && x <= 108))){
        printf(GREEN_TEXT);
    } else if (y == 14 && ((x >= 52 && x <= 68)|| (x >= 72 && x <= 88) || (x >= 92 && x <= 108))){
        printf(MAGENTA_TEXT);
    } 
}
