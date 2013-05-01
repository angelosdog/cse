//
//  Game.c
//  Milestone2
//
//  Created by Jiashu Chen on 2/05/12.
//  Copyright (c) 2012 University Of New South Wales. All rights reserved.
//

/*
 *  Game.c
 *  1917 v2.0
 *  Add to and change this file as you wish to implement the
 *  interface functions in Game.h
 *
 *  Created by Richard Buckland on 20/04/11.
 *  Copyright 2011 Licensed under Creative Commons SA-BY-NC 3.0.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "Game.h"

//Invalid vertexes and ARCs store this value.
#define INVALID -1

#define STUDENT_TYPES 6
#define TOTAL_X 150
#define TOTAL_Y 150
#define X_COORDINATE_RANGE 11
#define Y_COORDINATE_RANGE 11
#define TOTAL_ACTION 8

//Maximal coords
#define MAX_XY 5

#define START_X 2
#define START_Y 5
#define START_DIR 3

#define NUM_VERTEX 54
#define MAP_SIZE 11

#define VACANT 0
#define UNLUCKY 7

//Jiashu Edited 6:50 p.m. 22/MAY
#define AWARD_SPINOFF 3

//Positions of campusus
//These are array indices. Do not use without commenting that fact!
#define UNIA1 [2+5][5+5]
#define UNIA2 [-2+5][-5+5]

#define UNIB1 [-3+5][2+5]
#define UNIB2 [3+5][-2+5]

#define UNIC1 [5+5][3+5]
#define UNIC2 [-5+5][-3+5]

//For retraining
#define NUM_RTCS 9
#define STUDENT_ANY -1

typedef struct _score {
    int KPIs;
    int arcGrants;
    int publications;
    int campuses;
    int GO8Campus;
    int IPs;
} score;

/*
  typedef struct _Arcs{
  int startingPoint [X_COORDINATE_RANGE] [Y_COORDINATE_RANGE];
  int endingPoint [X_COORDINATE_RANGE] [Y_COORDINATE_RANGE];
  } Arcs;
*/

typedef struct most{
    int playerMost;
    int most;
} most;

typedef struct _game {
    int currentTurn;
    
    // Stores the resources of each uni
    // eg resources[UNI_A][STUDENT_MTV]
    int resources [NUM_UNIS + 1] [STUDENT_TYPES];

    // This will contain the scores for each player as a struct
    // eg scores[UNI_B].spinOffs
    score playerScores [NUM_UNIS + 1];

    // This will contain information about the board.
    int regionType [NUM_REGIONS];
    int regionValue [NUM_REGIONS];

    // If a player has a campus near a region a value is incremented.
    // The 20th region here is the ocean, which has no resources.
    int resIncome [NUM_UNIS + 1][NUM_REGIONS];

    //Index is from the magic co-ordinate system I didn't design
    int vertices [MAP_SIZE][MAP_SIZE];
    //Indexing destribed below
    int ARCS  [MAP_SIZE][MAP_SIZE][2];
    /* Left case
     * \
     *  X-
     * /
     * X is the index for the up left arc [0] and right arc [1]
     *
     * Right case
     *   /
     * -X
     *   \
     * X is the index for the upper right arc
     */

    most playerMost [TOTAL_ACTION];
} game;

//Used for accessing vertex and path info
typedef struct _map_point {
    int *ARC;
    int *vertex;

    //Regions
    int R1;
    int R2;
    int R3;
} point;

static void buildARC(Game g, action a, int player);
static void buildCampus(Game g, action a, int player);
static void buildGO8(Game g, action a, int player);

static point accessPath(Game g, path p);
static void setBoard(int vertex[MAP_SIZE][MAP_SIZE], int ARCs[MAP_SIZE][MAP_SIZE][2]);

// Not necesary anymore - Vincent 26/5/2012
//Jiashu Edited 6:24 p.m. 22/MAY
//static void spinOff (Game g, action a, int player);
//Finshed

//Actually adds resources
static void getIncome (Game g, int diceVal);
static int getRegion (int x, int y);


Game newGame (int discipline[], int dice[]) {
    Game g;
    int i;
    g = malloc(sizeof(game));
    g->currentTurn = -1;
    setBoard(g->vertices, g->ARCS);

    //Set dice values

    for (i = 0; i < NUM_REGIONS; i++) {
        g->regionValue[i] = dice[i];
	g->regionType[i] = discipline[i];
    }

    for (i = 0; i < TOTAL_ACTION; i++) {
        g->playerMost[i].most = NO_ONE;
        g->playerMost[i].playerMost = NO_ONE;
    }

    for (i=0; i <= NUM_UNIS; i++) {
        g->playerScores[i].arcGrants = 2;
        g->playerScores[i].campuses = 2;
        g->playerScores[i].GO8Campus = 0;
        g->playerScores[i].KPIs = 24;
        g->playerScores[i].publications = 0;
        g->playerScores[i].IPs = 0;
    }

    return g;
}

void disposeGame (Game g) {
    assert (g != NULL);
    free (g);
}

void makeAction (Game g, action a){
    int player = getWhoseTurn(g);   

    if (isLegalAction(g, a) == TRUE){
        if (a.actionCode == PASS) {
            // Q: How does one pass. There is another function for ending a
            // turn but it's done by runGame.c I assume it will be
            // handled externally
        } else if (a.actionCode == BUILD_CAMPUS){
    
            buildCampus(g, a, player);
            /* We don't need this for campuses! - Vincent 26/05/2012
            if (g->playerScores[getWhoseTurn(g)].campuses > g->playerMost[a.actionCode].most) {
                g->playerScores[g->playerMost [a.actionCode].playerMost].KPIs -= 10;
                g->playerMost[a.actionCode].playerMost = getWhoseTurn(g);
                g->playerMost[a.actionCode].most = g->playerScores[getWhoseTurn(g)].campuses;
        }
        */
            g->playerScores[player].campuses++;
        } else if (a.actionCode == BUILD_GO8){

            buildGO8(g, a, player);
            /* We don't need this! - Vincent 26/05/2012
            if (g->playerScores[player].GO8Campus > g->playerMost[a.actionCode].most) {
                g->playerMost[a.actionCode].playerMost = player;
                g->playerMost[a.actionCode].most = g->playerScores[player].GO8Campus;
            }
        */
            g->playerScores[player].GO8Campus++;
            g->playerScores[player].campuses--;
        } else if (a.actionCode == OBTAIN_ARC){

            buildARC(g, a, player);
            g->playerScores[player].arcGrants++;
            if (g->playerScores[player].arcGrants > g->playerMost[a.actionCode].most) {
                g->playerMost[a.actionCode].playerMost = player;
                g->playerMost[a.actionCode].most = g->playerScores[player].arcGrants;
            }

        /*} else if (a.actionCode == START_SPINOFF){
            // Edited by Vincent 26/5/2012
            According to the header file, START_SPINOFF is not a legal action. When a 
            player chooses to start a spinoff, the runGame.c file will change the 
            actionCode of a to OBTAIN_PUBLICATION or OBTAIN_IP_PATENT

            //Edited by Jiashu Chen 6:51 p.m. 22/MAY
            spinOff(g, a, player);

        } */

        } else if (a.actionCode == OBTAIN_PUBLICATION) {
            g->playerScores [player].publications++;


            if (g->playerScores[getWhoseTurn(g)].publications > g->playerMost[a.actionCode].most) {
                g->playerMost[a.actionCode].playerMost = getWhoseTurn(g);
                g->playerMost[a.actionCode].most = g->playerScores[getWhoseTurn(g)].publications;
            }

        } else if (a.actionCode == OBTAIN_IP_PATENT){
            g->playerScores [player].KPIs += 10;
            g->playerScores [player].IPs++;
        } else if (a.actionCode == RETRAIN_STUDENTS){

            g->resources[player][a.disciplineFrom]
                -=  getExchangeRate (g, player, a.disciplineFrom, a.disciplineTo);

            g->resources[player][a.disciplineTo] += 1;
        }  
    } else {
        printf("Invalid action.\n");
    }
}

void throwDice (Game g, int diceScore)
{
    g->currentTurn++;

    getIncome(g, diceScore);
}

int getTurnNumber (Game g) {
    return g->currentTurn;
}

// what type of students are produced by the specified region?
// see discipline codes above
int getDiscipline (Game g, int regionID){
    return g->regionType[regionID];
}

// what dice value produces students in the specified region?
// 2..12
int getDiceValue (Game g, int regionID){
    return g->regionValue[regionID];

}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started.
int getMostARCs (Game g){
    return g->playerMost[OBTAIN_ARC].playerMost;
}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is acquired after the game
// has started.
int getMostPublications (Game g){
    return g->playerMost[OBTAIN_PUBLICATION].playerMost;
}


int getWhoseTurn (Game g){

    return (g->currentTurn%3)+1;
}

int getKPIpoints (Game g, int player){
    return g->playerScores[player].KPIs;
}

int getARCs (Game g, int player){
    return g->playerScores[player].arcGrants;
}

int getGO8s (Game g, int player){
    return g->playerScores[player].GO8Campus;
}

int getCampuses (Game g, int player){
    return g->playerScores[player].campuses;
}

int getIPs (Game g, int player){
    return g->playerScores[player].IPs;
}

int getPublications (Game g, int player){
    return g->playerScores[player].publications;
}

int getStudents (Game g, int player, int discipline){
    return g->resources[player] [discipline];
}

// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.

//Patricks Fucntions:
static int doesPlayerOwnRTC(Game g, int player, int rtc) {
    int x, y;
    int result = FALSE;

 int rtcs[][3] = {

        {STUDENT_ANY,-2,3},
        {STUDENT_ANY,-3,2},

        {STUDENT_ANY,-4,0},
        {STUDENT_ANY,-3,-1},

        {STUDENT_ANY,-3,-5},
        {STUDENT_ANY,-2,-5},

        {STUDENT_ANY,-5,3},
        {STUDENT_ANY,5,2},

        {STUDENT_BPS,-4,-3},
        {STUDENT_BPS,-4,-4},

        {STUDENT_MJ,1,-3},
        {STUDENT_MJ,0,-4},

        {STUDENT_BQN,4,0},
        {STUDENT_BQN,3,-1},

        {STUDENT_MMONEY,4,4},
        {STUDENT_MMONEY,4,3},

        {STUDENT_MTV,0,4},
        {STUDENT_MTV,1,4},
    };

    x = rtcs[rtc][1];
    y = rtcs[rtc][2];

    if(g->vertices[x][y] == player
       || g->vertices[x][y] == player + 3) {
        result=TRUE;
    }
    return result;
}



int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo){

    int rtcs[][3] = {

        {STUDENT_ANY,-2,3},
        {STUDENT_ANY,-3,2},

        {STUDENT_ANY,-4,0},
        {STUDENT_ANY,-3,-1},

        {STUDENT_ANY,-3,-5},
        {STUDENT_ANY,-2,-5},

        {STUDENT_ANY,-5,3},
        {STUDENT_ANY,5,2},

        {STUDENT_BPS,-4,-3},
        {STUDENT_BPS,-4,-4},

        {STUDENT_MJ,1,-3},
        {STUDENT_MJ,0,-4},

        {STUDENT_BQN,4,0},
        {STUDENT_BQN,3,-1},

        {STUDENT_MMONEY,4,4},
        {STUDENT_MMONEY,4,3},

        {STUDENT_MTV,0,4},
        {STUDENT_MTV,1,4},
    };

    int cost=4;

    int i=0;
    // search through the RTCs to see if we own one that will
    // reduce the cost.
    for(i = 0; i < NUM_RTCS; i++) {
        if(doesPlayerOwnRTC(g, player, i)) {
            if(rtcs[i][0] == disciplineFrom) {		
		cost = 2;
	    }
	    else if(rtcs[i][0] == STUDENT_ANY
		       && cost > 2)   
            {
                cost = 3;
            }
        }
    }

    if(cost < 2) {
        cost = 2;
    }

    return cost;
}

//More functions
// Tristram Healy



int getCampus(Game g, path pathToVertex) {

    point mapPoint = accessPath(g, pathToVertex);

    return *(mapPoint.vertex);
}

int getARC(Game g, path pathToEdge) {

    point mapPoint = accessPath(g, pathToEdge);

    return *(mapPoint.ARC);
}

static void buildCampus(Game g, action a, int player) {

    point mapPoint = accessPath(g, a.destination);

    g->resIncome[player][mapPoint.R1] += 1;
    g->resIncome[player][mapPoint.R2] += 1;
    g->resIncome[player][mapPoint.R3] += 1;

    *(mapPoint.vertex) = player;
}

static void buildGO8(Game g, action a, int player) {
    point mapPoint = accessPath(g, a.destination);

    g->resIncome[player][mapPoint.R1] += 1;
    g->resIncome[player][mapPoint.R2] += 1;
    g->resIncome[player][mapPoint.R3] += 1;
    
    *(mapPoint.vertex) = player + 3;
}

static void buildARC(Game g, action a, int player) {

    point mapPoint = accessPath(g, a.destination);

    *(mapPoint.ARC) = player;
}

int isLegalAction(Game g, action a) {

    int out = FALSE;
    path place;
    path placeL;
    path placeR;
    path placeB;
    point p, pl, pr, pb;
    int player = getWhoseTurn(g);

    // Aparenetly we don't need to check if a path is valid
    // I've spent too many darn hours doing that exact thing so I
    // left it here. However I've stopped doing it half way through
    strcpy(place, a.destination);
    strcpy(placeL, a.destination);
    strcpy(placeR, a.destination);
    strcpy(placeB, a.destination);
    strcat(placeL, "L");
    strcat(placeR, "R");
    strcat(placeB, "B");

    p = accessPath(g, place);
    pl = accessPath(g, placeL);
    pr = accessPath(g, placeR);
    pb = accessPath(g, placeB);

    if (a.actionCode == PASS) {
        out = TRUE;
    } else if( a.actionCode == BUILD_CAMPUS) {

        //Plausible Bug. Possible NULL dereference.
        //To fix you just need to nest: if(*(p.vertex) == -1)
        if (p.vertex == NULL ||  *(p.vertex) == -1) {
            out = FALSE;
        } else if (pl.vertex != NULL && *(pl.vertex) != VACANT) {
            out = FALSE;
        } else if (pr.vertex != NULL && *(pr.vertex) != VACANT) {
            out = FALSE;
        } else if (pr.vertex != NULL && *(pr.vertex) != VACANT) {
            out = FALSE;
        } else if (pr.ARC != NULL) {
            if (*(pr.ARC) != INVALID && *(pr.ARC) == player) {
                out = FALSE;
            }
        } else if (pl.ARC != NULL) {
            if (*(pl.ARC) != INVALID && *(pl.ARC) == player) {
                out = FALSE;
            }
        } else if (pb.ARC != NULL) {
            if (*(pb.ARC) != INVALID && *(pb.ARC) == player) {
                out = FALSE;
            }
        } else if (g->resources [player] [STUDENT_BPS] >= 1
		   && g->resources [player] [STUDENT_BQN] >= 1
		   && g->resources [player] [STUDENT_MTV] >=1
		   && g->resources [player] [STUDENT_MJ] >= 1) {
            //Jiashu Edited 6:16 p.m. 22/MAY
            out = TRUE;
        } else {
            out = FALSE;
        }
        //Finish
    } else if (a.actionCode == BUILD_GO8 ) {

        if (p.vertex != NULL && *(p.vertex) == player) {
            out = TRUE;
        } else {
            out = FALSE;
        }

        //Jiashu Edited 6:19 p.m. 22/MAY
        if (g->resources [player] [STUDENT_MMONEY] >= 3
            && g->resources [player] [STUDENT_MJ] >= 2
            && out == TRUE) {
            out = TRUE;
        } else {
            out = FALSE;
        }
        //Finish


    } else if (a.actionCode == OBTAIN_ARC) {

        //From now on I'm assuming the PATH is legal
        if (p.ARC == NULL || *(p.ARC) == INVALID) {
            out = FALSE;
        } else {

            //Jiashu Edited 6:20 p.m. 22/MAY
            if (g->resources [player] [STUDENT_BPS] >= 1
                && g->resources [player] [STUDENT_BQN] >= 1) 
	    {
                out = TRUE;
            } else {
                out = FALSE;
            }
            //Finish
        }

    } else if (a.actionCode == START_SPINOFF) {
        //Jiashu Edited 6:16 p.m. 22/MAY
        if (g->resources [player] [STUDENT_MMONEY] >= 1
            && g->resources [player] [STUDENT_MJ] >= 1
            && g->resources [player] [STUDENT_MTV] >= 1) {

            out = TRUE;
        }

    } else if (a.actionCode == RETRAIN_STUDENTS) {

        //If the player has enough students to convert
        if (g->resources [player] [a.disciplineFrom]
            > getExchangeRate(g, player, a.disciplineFrom,
			      a.disciplineTo))
        {
            out = TRUE;

        } else {
            out = FALSE;
        }

    } else {
        out = FALSE;
    }

    // Check if there are too many GO8s - Vincent 26/5/2012 
    int totalGO8s;
    totalGO8s = g->playerScores[UNI_A].GO8Campus + g->playerScores[UNI_B].GO8Campus + g->playerScores[UNI_C].GO8Campus;
    if (totalGO8s == 8 && a.actionCode == BUILD_GO8){
        out = FALSE;   
    }
    return out;
}

/* Not necessary anymore - Vincent 26/5/2012
// edited by Jiashu Chen 6:30 p.m. 22/MAY
static void spinOff (Game g, action a, int player){
    int determine;
    determine = rand() % 3 + 1;

    if (determine == 1 || determine == 2) {
        g->playerScores [player].publications++;

        //update for Spin off function has been moved from makeAction to here by Jiashu Chen
        // 6:54 p.m. 22/MAY

        if (g->playerScores[getWhoseTurn(g)].publications > g->playerMost[a.actionCode].most) {
            g->playerMost[a.actionCode].playerMost = getWhoseTurn(g);
            g->playerMost[a.actionCode].most = g->playerScores[getWhoseTurn(g)].publications;
        }

    } else {
        g->playerScores [player].KPIs += 10;
        g->playerScores [player].IPs++;
    }
}
*/

//Tristram Healy
/* Direction indexing
 *
 *  0   1
 *   \ /
 * 5--X--2
 *   / \
 *  4   3
 *
 */

//Indexing for ARCS (it's copied to here so I can write the code)
/* Left case
 * \
 *  X-
 * /
 * X is the index for the up left arc [0] and right arc [1]
 *
 * Right case
 *   /
 * -X
 *   \
 * X is the index for the upper right arc [0]. [1] is is not used.
 */

/* Regions:
 * Not used to index the info, just to access it so it can be changed
 * when a campus is built.
 *
 * Even dir:
 * 1) x      y + 1
 * 2) x + 1  y
 * 3) x - 1  y - 1
 *
 * Odd dir:
 * 1) x + 1  y + 1
 * 2) x      y - 1
 * 3) x - 1  y
 */


static point accessPath(Game g, path p) {
    point out;
    int i;
    int dirs[6][2] = { { 0, 1}, { 1, 1}, { 1, 0},
                       { 0,-1}, {-1,-1}, {-1, 0} };
    int dir = START_DIR;
    int x = START_X;
    int y = START_Y;

    // 6 and 3 represent the vertices of a hexagon.
    // (there are 6 directions you can move in)
    for (i = 0; p[i] != '\0'; i++) {
        if(p[i] == 'L') {
            // 5 is congruent to -1 mod 6
            dir = (dir + 5) % 6;
        } else if(p[i] == 'R') {
            dir = (dir + 1) % 6;
        } else {
            // Turn around
            dir = (dir + 3) % 6;
        }

        x += dirs[dir][0];
        y += dirs[dir][1];

        i++;
    }

    if(x < -MAX_XY || y < -MAX_XY  || x > MAX_XY || y > MAX_XY) {
        out.ARC = NULL;
        out.vertex = NULL;
    } else {
        x += 5;
        y += 5;
        out.vertex = &(g->vertices[x][y]);
        if(dir == 0) {
            out.ARC = &(g->ARCS[x][y-1][0]);
        } else if(dir == 1) {
            out.ARC = &(g->ARCS[x-1][y-1][0]);
        } else if(dir == 2) {
            out.ARC = &(g->ARCS[x-1][y][1]);
        } else if(dir == 3) {
            out.ARC = &(g->ARCS[x][y][0]);
        } else if(dir == 4) {
            out.ARC = &(g->ARCS[x][y][0]);
        } else {
            out.ARC = &(g->ARCS[x][y][1]);
        }
    }

    //Regions
    if (dir % 2 == 0) {
        out.R1 = getRegion(x,   y+1);
        out.R2 = getRegion(x+1, y);
        out.R3 = getRegion(x-1, y-1);
    } else {
        out.R1 = getRegion(x+1, y+1);
        out.R2 = getRegion(x,   y-1);
        out.R3 = getRegion(x-1, y);
    }

    return out;
}


static void setBoard(int vertex[MAP_SIZE][MAP_SIZE], int ARCs[MAP_SIZE][MAP_SIZE][2])
{

    int i, x, y, arcs;

    // Set all vertex's and ARCS to illegal (-1)
    memset(vertex, -1, 10*10*sizeof(int));
    memset(ARCs, -1, 10*10*2*sizeof(int));

    // List of legal vertices and how many arcs they represent
    // eg {0,1,2} means the vertex at coord 0,1, which has 2 arcs
    // reference by it.
    // A vertex listed to have 3 ARCS is a special case
    // It is a vertex with 1 coord but has no [0] arc only [1]
    // There are 54 vertices in total
    int map[54][3] = { { 0, 1, 2}, { 1, 1, 1}, { 0,-1, 1},
                       {-1,-1, 2}, {-1, 0, 1}, { 0, 2, 1},
                       { 1, 3, 2}, { 2, 3, 1}, { 2, 3, 2},
                       { 3, 2, 1}, { 3, 1, 2}, { 2, 0, 1},
                       { 2,-1, 2}, { 1,-2, 1}, { 0,-2, 2},
                       { 0,-2, 2}, {-1,-3, 1}, {-2,-3, 2},
                       {-2,-2, 1}, {-3,-2, 2}, {-2,-1, 1},
                       {-2, 0, 2}, {-2, 1, 1}, {-1, 2, 2},
                       {-1, 3, 1}, { 0, 4, 3}, { 1, 4, 2},
                       { 2, 5, 3}, { 3, 5, 0}, { 3, 4, 2},
                       { 4, 4, 0}, { 4, 3, 2}, { 5, 4, 0},
                       { 5, 3, 1}, { 4, 1, 1}, { 4, 0, 1},
                       { 3,-1, 1}, { 3,-2, 1}, { 2,-3, 1},
                       { 1,-3, 2}, { 0,-4, 1}, {-1,-4, 2},
                       {-2,-5, 1}, {-3,-5, 2}, {-3,-4, 1},
                       {-4,-4, 2}, {-4,-3, 1}, {-5,-3, 2},
                       {-5,-2, 1}, {-3,-1, 2}, {-4, 0, 1},
                       {-3, 1, 2}, {-3, 2, 1}, {-2, 3, 3} };



    for (i = 0; i < NUM_VERTEX; i++) {
        x = map[i][0];
        y = map[i][1];
        arcs = map[i][2];

        // All coords are offset by 5
        vertex[x+5][y+5] = VACANT_VERTEX;

        if (arcs == 1) {
            ARCs[x+5][y+5][0] = VACANT_ARC;
        } else if(arcs == 2) {
            ARCs[x+5][y+5][0] = VACANT_ARC;
            ARCs[x+5][y+5][1] = VACANT_ARC;
        } else { // arcs == 3
            ARCs[x+5][y+5][1] = VACANT_ARC;
        }
    }

    //!!!
    //UNIxx are #defines of array indicies
    //!!!

    //Set player starting positions
    //UNI_A

    vertex UNIA1 = CAMPUS_A;
    ARCs [1+5][4+5][0] = ARC_A;
    vertex UNIA2 = CAMPUS_A;
    ARCs UNIA2[0] = ARC_A;

    //UNI_B
    vertex UNIB1 = CAMPUS_B;
    ARCs [-3+5][1+5][0] = ARC_B;
    vertex UNIB2 = CAMPUS_B;
    ARCs [3 + 5][-2 + 5][0] = ARC_B;

    //UNI_C
    vertex UNIC1 = CAMPUS_C;
    ARCs UNIC1 [1] = ARC_C;
    vertex UNIC2 = CAMPUS_C;
    ARCs [4+5][3+5][1] = ARC_C;

}

static void getIncome (Game g, int diceVal)
{
    int i;
    int player = getWhoseTurn(g);

    // For each region
    for (i = 0; i < NUM_REGIONS; i++) {
        // check if it's diceValue matches the roll
        if (getDiceValue(g, i) == diceVal) {
            // if it does, add the apropriate resource
            g->resources[player][getDiscipline(g, i)]
                += g->resIncome [player] [i];
        }
    }

    if (diceVal == UNLUCKY)
    {
        g->resources[player][STUDENT_THD] +=
            g->resources[player][STUDENT_MTV]
            + g->resources[player][STUDENT_MMONEY];

        g->resources[player][STUDENT_MTV] = 0;
        g->resources[player][STUDENT_MMONEY] = 0;

    }

}


static int getRegion(int x, int y) {
    int regions[][2] = {
        {-2, 2}, {-3, 0}, {-4,-2}, { 0, 3}, {-1, 1},
        {-1,-1}, {-3,-3}, { 2, 4}, { 1, 2}, { 0, 0},
        {-1,-2}, {-2,-4}, { 3, 3}, { 2, 1}, { 1,-1},
        { 0,-3}, { 4, 2}, { 3, 0}, { 2,-2}
    };
    //Defaults to the blackhole region.
    int out = NUM_REGIONS;
    int i;
    for (i = 0;i < NUM_REGIONS; i++) {
        if (regions[i][0] == x
            && regions[i][1] == y)
        {
            out = i;
        }
    }
    return out;
}

