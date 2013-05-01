#include <stdio.h>
#include <stdlib.h>

#define WIDTH_OF_BOARD 115
#define HEIGHT_OF_BOARD 32
#define NUMBER_OF_REGIONS 19

#define RED "\x1b[41m" //BPS
#define GREEN "\x1b[42m" //MTV
#define YELLOW "\x1b[43m" //MJ
#define BLUE "\x1b[44m" //THD
#define MAGENTA "\x1b[45m" //MMON
#define CYAN "\x1b[46m" //BQN
#define DEFAULT "\x1b[0m"

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
   for (i = 0; i < HEIGHT_OF_BOARD; i++){
       for (j = 0; j < WIDTH_OF_BOARD; j++){
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
    int disciplines [19] = {5,3,3,2,4,3,1,1,4,0,2,1,5,3,5,4,2,2,4}; 
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

void drawTable(Board b){

    /*
    We will use the following table as a an outline for the score table
 
     |-----------------------------------------------------------|
     | UNI_A             | UNI_B             | UNI_C             |
     |-------------------|-------------------|-------------------|
     | KPI: %3d          | KPI: %3d          | KPI: %3d          |
     | Campuses: %3d     | Campuses: %3d     | Campuses: %3d     |
     | GO8s: %3d         | GO8s: %3d         | GO8s: %3d         |
     | ARC Grants: %3d   | ARC Grants: %3d   | ARC Grants: %3d   |
     | IPs: %3d          | IPs: %3d          | IPs: %3d          |
     | Publications: %3d | Publications: %3d | Publications: %3d |
     | THDs: %3d         | THDs: %3d         | THDs: %3d         |
     | BPs: %3d          | BPs: %3d          | BPs: %3d          |
     | BQNs: %3d         | BQNs: %3d         | BQNs: %3d         |
     | MJs: %3d          | MJs: %3d          | MJs: %3d          |
     | MTVs: %3d         | MTVs: %3d         | MTVs: %3d         |
     | MMONEY: %3d       | MMONEY: %3d       | MMONEY: %3d       |
     |-----------------------------------------------------------|

     */
}
