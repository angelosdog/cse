#include <stdio.h>
#include <stdlib.h>

#define WIDTH_OF_BOARD 115
#define HEIGHT_OF_BOARD 32
#define NUMBER_OF_REGIONS 19

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


// disciplines
#define STUDENT_THD 0
#define STUDENT_BPS 1
#define STUDENT_BQN 2
#define STUDENT_MJ  3
#define STUDENT_MTV 4
#define STUDENT_MMONEY 5



typedef struct _board* Board;
typedef struct _board{
    char gameBoard [HEIGHT_OF_BOARD][WIDTH_OF_BOARD];
} board;
Board createBoard(void);
void printBoard(Board b);
int findCentreX(int regionID);
int findCentreY(int regionID);
void drawRegion(Board b, int centreX, int centreY, int regionID);
void drawTable(Board b);
void colourMap(Board b, int x, int y, int disciplines[]);
void colourTable(Board b, int x, int y);
char* disciplineOfRegion (int regionID, int disciplines[]);

int main(int argc, char* argv[]){
    Board b;
    b = createBoard();
    // print out board on a quadrilateral grid
    printBoard(b);
    return 0;
}

Board createBoard(void){
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

void printBoard (Board b){
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
   for (regionID = 0; regionID < NUMBER_OF_REGIONS; regionID++){
       centreX = findCentreX(regionID);
       centreY = findCentreY(regionID);
       drawRegion(b, centreX, centreY, regionID);    
   }
   drawTable(b);
   int i;
   int j;
   int disciplines [19] = {5,3,3,2,4,3,1,1,4,0,2,1,5,3,5,4,2,2,4}; 

   for (i = 0; i < HEIGHT_OF_BOARD; i++){
       for (j = 0; j < WIDTH_OF_BOARD; j++){
           if (j < 50){
               colourMap(b, j, i, disciplines);
           } else {
               colourTable(b, j, i);
           }
           printf("%c", b->gameBoard[i][j]);
       }
       printf("\n");
   } 
}

int findCentreY(int regionID){
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

int findCentreX(int regionID){
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

void drawRegion(Board b, int x, int y, int regionID){
    // break a hexagon into 13 parts; its 6 edges, its 6 vertices
    // and the centre
    // start from the centre, then the rightmost vertex, and then work clockwise
    char temp[2];
    sprintf(temp, "%d", regionID);

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

void colourMap(Board b, int x, int y, int disciplines[]){
    // In case the node we are checking is not in the hexagon
    printf(DEFAULT);
    // Find the centres of each hexagon, iterate around that hexagon and change colour accordingly
    int centreX;
    int centreY;
    int regionID;
    int row;
    int column;
    char* colour;
    for (regionID = 0; regionID < NUMBER_OF_REGIONS; regionID++){
        centreX = findCentreX(regionID);
        centreY = findCentreY(regionID);
        for (row = centreX-2; row < centreX+4; row++){
            column = centreY+2;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, disciplines);
                printf(colour);
            }
        }
        for (row = centreX-3; row < centreX+5; row++){
            column = centreY+1;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, disciplines);
                printf(colour);
            }
        }
        for (row = centreX-4; row < centreX+6; row++){
            column = centreY;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, disciplines);
                printf(colour);
            }
        }
        for (row = centreX-3; row < centreX+5; row++){
            column = centreY-1;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, disciplines);
                printf(colour);
            }
        }
        for (row = centreX-2; row < centreX+4; row++){
            column = centreY-2;
            if (row == x && column == y && b->gameBoard[y][x] == ' '){
                colour = disciplineOfRegion(regionID, disciplines);
                printf(colour);
            }
        }
 
    }
   
}

char* disciplineOfRegion (int regionID, int disciplines[]){
    char* colour = DEFAULT;
    if (disciplines[regionID] == STUDENT_THD){
        colour = BLUE_BACKGROUND;
    } else if (disciplines[regionID] == STUDENT_BPS) {
        colour = RED_BACKGROUND;
    } else if (disciplines[regionID] == STUDENT_BQN) {
        colour = CYAN_BACKGROUND;
    } else if (disciplines[regionID] == STUDENT_MJ) {
        colour = YELLOW_BACKGROUND;
    } else if (disciplines[regionID] == STUDENT_MTV) {
        colour = GREEN_BACKGROUND;
    } else if (disciplines[regionID] == STUDENT_MMONEY) {
        colour = MAGENTA_BACKGROUND;
    }
    
    return colour;
}

void drawTable(Board b){

    sprintf(&(b->gameBoard[0][50]),  "|-------------------|-------------------|-------------------|");
    sprintf(&(b->gameBoard[1][50]),  "| UNI_A             | UNI_B             | UNI_C             |");
    sprintf(&(b->gameBoard[2][50]),  "|-------------------|-------------------|-------------------|");
    sprintf(&(b->gameBoard[3][50]),  "| KPI:          %3d | KPI:          %3d | KPI:          %3d |", 150, 24, 100);
    sprintf(&(b->gameBoard[4][50]),  "| Campuses:     %3d | Campuses:     %3d | Campuses:     %3d |", 6, 2, 3);
    sprintf(&(b->gameBoard[5][50]),  "| GO8s:         %3d | GO8s:         %3d | GO8s:         %3d |", 1, 0, 0);
    sprintf(&(b->gameBoard[6][50]),  "| ARC Grants:   %3d | ARC Grants:   %3d | ARC Grants:   %3d |", 24, 2, 15);
    sprintf(&(b->gameBoard[7][50]),  "| IPs:          %3d | IPs:          %3d | IPs:          %3d |", 3, 0, 1);
    sprintf(&(b->gameBoard[8][50]),  "| Publications: %3d | Publications: %3d | Publications: %3d |", 20, 0, 2);
    sprintf(&(b->gameBoard[9][50]),  "| THDs:         %3d | THDs:         %3d | THDs:         %3d |", 4, 150, 40);
    sprintf(&(b->gameBoard[10][50]), "| BPs:          %3d | BPs:          %3d | BPs:          %3d |", 3, 0, 4);
    sprintf(&(b->gameBoard[11][50]), "| BQNs:         %3d | BQNs:         %3d | BQNs:         %3d |", 5, 0, 10);
    sprintf(&(b->gameBoard[12][50]), "| MJs:          %3d | MJs:          %3d | MJs:          %3d |", 6, 0, 2);
    sprintf(&(b->gameBoard[13][50]), "| MTVs:         %3d | MTVs:         %3d | MTVs:         %3d |", 7, 0, 3);
    sprintf(&(b->gameBoard[14][50]), "| MMONEY:       %3d | MMONEY:       %3d | MMONEY:       %3d |", 2, 0, 4);
    sprintf(&(b->gameBoard[15][50]), "|-------------------|-------------------|-------------------|"); 
}

void colourTable(Board b, int x, int y){
    printf(DEFAULT);
    
    if (y == 1 && (x >= 52 && x <= 68)){
        printf(RED_TEXT);
    } else if (y == 1 && (x >= 72 && x <= 88)){
        printf(CYAN_TEXT);
    } else if (y == 1 && (x >= 92 && x <= 108)){
        printf(MAGENTA_TEXT);
    } else if (y == 9 && ((x >= 52 && x <= 68)|| (x >= 72 && x <= 88) || (x >= 92 && x <= 108))){
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
