#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Game.h"

/*********************** Player.h *************************************/
typedef struct player *Player;

#define NUM_DISCIPLINES 6

#define INITIAL_NUM_CAMPUSES 2
#define CAMPUS_POINTS 10
#define GO8_POINTS 10
#define INITIAL_NUM_ARCS 2
#define ARC_POINTS 2
#define PATENT_POINTS 10
#define MOST_ARC_POINTS 10
#define MOST_PUBS_POINTS 10

struct player {
  int playerNum;
  int kpiPoints;
  int numArcs;
  int numGO8s;
  int numCampuses;
  int numIPs;
  int numPubs;
  int students[NUM_DISCIPLINES];
  int numUnusedSpinoffs;
};

// returns a new player with the given player number
Player newPlayer(int playerNum);

// return the number of KPI points the specified player currently has
int KPIpoints (Player player);

// return the number of ARC grants the specified player currently has
int ARCs (Player player);

// return the number of GO8 campuses the specified player currently has
int GO8s (Player player);

// return the number of normal Campuses the specified player currently has
int campuses (Player player);

// return the number of IP Patents the specified player currently has
int IPs (Player player);

// return the number of Publications the specified player currently has
int publications (Player player);

// return the number of students of the specified discipline type
// the specified player currently has
int students (Player player, int discipline);

// returns whether the player has enough students to
// perform the specified action
int enoughStudents (Player player, int actionCode);

// give the player the specified number of students of the given
// discipline
void giveStudents (Player player, int discipline, int numStudents);

// deduct the player the number of students for the required
// action code. also award any kpi points/spinoffs
// to the given player for the given action.
void deductStudents (Player player, int actionCode);

// give the player a publication
void givePublication(Player player);

// give the player a patent
void givePatent(Player player);

// convert MTV & MMONEY students of all players to THDs
void convertStudentsToTHDs(Player *players);

// awards the points for most arc grants. returns the number of the
// player with the most arc grants
int awardMostArcs(Player *players, int playerWithMostArcs,
		  int firstArcAward);

// awards the points for most publications. returns the number of the
// player with the most publications
int awardMostPubs(Player *players, int playerWithMostPubs,
		  int firstPubsAward);

// get the total number of GO8s owned by all the players
int totalGO8s(Player *players);

// get the total number of unused spinoffs a player has
int getUnusedSpinoffs(Player player);

// convert from player number to relevant campus code
int playerToCampus(int player);

// convert from campus code to player number
int campusToPlayer(int campus);

// convert from player number to relevant GO8 code
int playerToGO8(int player);

// convert from player number to relevant arc code
int playerToArc(int player);

// get the player with number playerNum from the players array
Player getPlayer(Player players[], int playerNum);

int howManyStudents(int actionCode, int discipline);

void disposePlayers(Player players[]);

/************************ Board.h ************************************/

// retraining centre types
#define TWO_TO_ONE_MTV 0
#define TWO_TO_ONE_BQN 1
#define TWO_TO_ONE_BPS 2
#define TWO_TO_ONE_MJ 3
#define TWO_TO_ONE_MMONEY 4
#define THREE_TO_ONE_ANY 5

typedef struct board *Board;

// return the tiles arrray from the given board

// return the contents of the given vertex (ie campus code or
// VACENT_VERTEX)
int getVertexContents(Board board, path pathToVertex);

// the contents of the given edge (ie ARC code or vacent ARC)
int getEdgeContents(Board board, path pathToEdge);

// Create a new board with given disciplines & dice values
Board newBoard(int discipline[], int dice[]);

// returns whether the given player owns an arc adjacent to the
// vertex specified by destination
int adjacentArcToVertex(Board board, path destination, int player);

// returns whether the given player owns an arc adjacent to the
// edge specified by destination
int adjacentArcToEdge(Board board, path destination, int player);

// returns whether the given player owns a campus adjacent to the
// destination
int adjacentCampus(Board board, path destination, int player);

// returns whether the given player owns a campus adjacent to the
// edge specified by "destination"
int adjacentCampusToEdge(Board board, path destination, int player);

// returns whether *anyone* owns a campus adjacent to the destination
int adjacentCampuses(Board board, path destination);

// generate students for the given dice score
void generateStudents(Board board, Player players[], int diceScore);

// build the specified thing for the given player
void build(Board board, path destination,
           int actionCode, int playerNum);

// return whether the given player has access to a retraining
// centre of the given type
int hasRetrainingCentre(Board board, int player, int centreType);

// return whether the specified path is a valid path
int isValidPath(Board board, path destination) ;

void disposeBoard(Board board);

/************************ Vertex.h ***********************************/
/*
Edge Orientations
                         __
     /                                      \
 North-East (NE)     East-West (EW)    South-East (SE)
*/

typedef int Orientation;

#define UNDEFINED -1
#define EW 0
#define SE 1
#define NE 2

// Number of edges per vertex
#define NUM_EDGES 3

typedef struct vertex *Vertex;

typedef struct edge *Edge;

// make a new vertex
Vertex newVertex(void);

// gets a vertex given the path.
// saves the last edge in the path as the vertex's
// 'requested edge' field.
Vertex getVertex(path pathToDest, Vertex start, int *success);

Edge getEdge(path pathToDest, Vertex start, int *success);

Vertex getEdgeTo(Edge e);

Vertex getEdgeFrom(Edge e);

// connect two vertexes together, with given edge direction
static void connect(Vertex a, Vertex b, int direction);

// get the number of connections a vertex has
int getNumConnections(Vertex vertex);

// get the campus type that the vertex is holding
int getCampusType(Vertex vertex);

// set the campus type of the given vertex to the given value
void setCampusType(Vertex vertex, int campusType);

// get the arc type of the vertex's requested edge (if it was the
// last vertex in a specified path)
int getArcType(Vertex vertex);

// set the arc type of the given vertex's requested edge to the given value
void setArcType(Vertex vertex, int arcType);

// returns whether the given player has an adjacent arc to
// the given edge
int hasAdjacentArc(Edge edge, int player);

// returns whether the given player has an adjacent campus to
// the given vertex
int hasAdjacentCampus(Vertex vertex, int player);


// make a new edge connecting vertexes 'to' and 'from'
// with given direction
Edge newEdge(Vertex to, Vertex from, int direction);

// get the edge with the orientation of the direction that we're
// going to be travelling when moving to the next vertex
Edge getEdgeWithOrientation(Vertex nextVertex, int nextMove, int *success);

// get the next move given the direction and the last move
int getNextMove(char direction, int lastMove);

// get the next vertex given the current vertex and the
// direction of the next move
Vertex getNextVertex(Vertex currentVertex, int nextMove, int *success);

// tells whether two vertexes are connected
int connected(Vertex a, Vertex b);

// returns whether the player owns an arc attached to that vertex
int playerOwnsArc(Vertex vertex, int player);

void disposeVertex(Vertex vertex);


/************************ Tile.h **************************************/
/*
  Numbering of vertices on hexagons

        0__1
      5/    \2
       \ __ /
        4   3
*/

// Number of corners per tile
#define NUM_CORNERS 6

// Number of columns & rows on gameboard
#define NUM_COLUMNS 5
#define NUM_ROWS 5

// Starting tile & vertex for paths (as described in project spec)
#define START_TILE 7
#define START_VERTEX 0

typedef struct tile *Tile;

// Create a new tile array (the board)
Tile *createTiles(int discipline[], int dice[]);

// Gets the starting vertex for paths (as described in the project spec)
Vertex getStartVertex(Tile *tiles);

// Gets all vertexes of the tile as an array
Vertex *getVertexes(Tile tile);

// Returns the discipline of students that the tile generates
int getTileDiscipline(Tile tile);

// Returns the dice number of the tile
int getDiceNumber(Tile tile);

void disposeTiles(Tile tiles[]);

Tile * getTiles(Board board);


/************************ Game stuff ***********************************/
// roll which all students get converted to THDs
#define MAGIC_ROLL 7

// can only be 8 GO8 campuses
#define MAX_GO8S 8

struct _game {
  int turn;
  Board board;
  Player players[NUM_UNIS];
  int awardMostArcs;
  int awardMostPubs;
  int playerWithMostArcs;
  int playerWithMostPubs;
};

/************************ Input Validation ****************************/


static int validatePlayer(int player) {
  if (!(player == UNI_A || player == UNI_B || player == UNI_C)) {
    fprintf(stderr, " ** Invalid player number: %d\n", player);
#ifndef SUPERVISOR
    assert(!"Invalid player number!");
#endif
    return FALSE;
  }
  return TRUE;
}

static int validatePath(Board board, path destination) {
  if (!isValidPath(board, destination)) {
    fprintf(stderr, " ** Invalid path string: %s\n", destination);
#ifndef SUPERVISOR
    assert(!"Invalid path string!");
#endif
    return FALSE;
  }
  return TRUE;
}

static int validateDiscipline(int discipline) {
  if (!(discipline == STUDENT_THD || discipline == STUDENT_BQN
         || discipline == STUDENT_BPS || discipline == STUDENT_MTV
	|| discipline == STUDENT_MJ || discipline == STUDENT_MMONEY)) {
    fprintf(stderr, " ** Invalid discipline number: %d\n", discipline);
#ifndef SUPERVISOR
    assert(!"Invalid discipline number!");
#endif
    return FALSE;
  }
  return TRUE;
}

/************************ Game.c *************************************/

int getDiscipline(Game g, int regionID) {
  Tile * tiles = getTiles(g->board);
  if (!(regionID >= 0 && regionID < NUM_REGIONS)) {
    fprintf(stderr, " ** Invalid region ID: %d\n", regionID);
#ifndef SUPERVISOR
    assert(!"Invalid region ID!");
#endif
    return 0;
  }
  return getTileDiscipline(tiles[regionID]);
}

int getDiceValue(Game g, int regionID) {
  Tile * tiles = getTiles(g->board);
  if (!(regionID >= 0 && regionID < NUM_REGIONS)) {
    fprintf(stderr, " ** Invalid region ID: %d\n", regionID);
#ifndef SUPERVISOR
    assert(!"Invalid region ID!");
#endif
    return 2;
  }
  return getDiceNumber(tiles[regionID]);
}

Game newGame(int discipline[], int dice[]) {
  Game game = malloc(sizeof (struct _game));
  if (game == NULL) {
    fprintf(stderr, " ** malloc failed in Game.c :(\n");
    assert(FALSE);
  }
  game->turn = -1;
  game->board = newBoard(discipline, dice);
  int i = 0;
  while (i < NUM_UNIS) {
    game->players[i] = newPlayer(i);
    i++;
  }
  // no-one gets awarded anything until someone has
  // built an arc or obtained a publication
  game->awardMostArcs = FALSE;
  game->awardMostPubs = FALSE;
  game->playerWithMostPubs = NO_ONE;
  game->playerWithMostArcs = NO_ONE;
  return game;
}

int isLegalAction (Game g, action a) {
  int legal = FALSE;
  int code = a.actionCode;
  Player currentPlayer = getPlayer(g->players, getWhoseTurn(g));
  int currentPlayerCampus = playerToCampus(getWhoseTurn(g));
  Board board = g->board;
  if (code == PASS) {
    legal = TRUE;
  } else if (code == BUILD_CAMPUS) {
    if (!validatePath(g->board, a.destination)) {
      legal = FALSE;
    } else if (!adjacentCampuses(board, a.destination)
	       && adjacentArcToVertex(board, a.destination, getWhoseTurn(g))
	       && getCampus(g, a.destination) == VACANT_VERTEX
	       && enoughStudents(currentPlayer, BUILD_CAMPUS)) {
      legal = TRUE;
    }
  } else if (code == OBTAIN_ARC) {
    if (!validatePath(g->board, a.destination)) {
      legal = FALSE;
    } else if ((adjacentArcToEdge(board, a.destination, getWhoseTurn(g))
		|| adjacentCampusToEdge(board, a.destination, getWhoseTurn(g)))
	       && (getARC(g, a.destination) == VACANT_ARC)
	       && enoughStudents(currentPlayer, OBTAIN_ARC)) {
      legal = TRUE;
    }
  } else if (code == BUILD_GO8) {
    if (!validatePath(g->board, a.destination)) {
      legal = FALSE;
    } else if ((getCampus(g, a.destination) == currentPlayerCampus)
	       && enoughStudents(currentPlayer, BUILD_GO8)
	       && (totalGO8s(g->players) < MAX_GO8S)) {
      legal = TRUE;
    }
  } else if (code == START_SPINOFF) {
    if (enoughStudents(currentPlayer, START_SPINOFF)) {
      legal = TRUE;
    }
  } else if ((code == OBTAIN_PUBLICATION) ||
	     (code == OBTAIN_IP_PATENT)) {
    // behaviour is undefined
    legal = FALSE;
  } else if (code == RETRAIN_STUDENTS) {
    if (!(a.disciplineFrom == STUDENT_THD || a.disciplineFrom == STUDENT_BQN
	     || a.disciplineFrom == STUDENT_BPS || a.disciplineFrom == STUDENT_MTV
	  || a.disciplineFrom == STUDENT_MJ || a.disciplineFrom == STUDENT_MMONEY)) {
      legal = FALSE;
    } else if (a.disciplineFrom == STUDENT_THD) {
      legal = FALSE;
    } else {
      int exchangeRate = getExchangeRate (g, getWhoseTurn(g),
					  a.disciplineFrom,
					  a.disciplineTo);
      if (students(currentPlayer, a.disciplineFrom) >= exchangeRate) {
	legal = TRUE;
      }
    }
  }
  return legal;
}

// return the current turn number of the game 0,1, ..
int getTurnNumber(Game game) {
  return game->turn;
}

// advance the game to the next turn, assuming that the dice has just
// been dice has just been rolled and produced diceScore
void throwDice(Game game, int diceScore) {
  game->turn++;
  generateStudents(game->board, game->players, diceScore);
  // if a seven was thrown, convert the necessary students to THDs
  if (diceScore == MAGIC_ROLL) {
    convertStudentsToTHDs(game->players);
  }
}

// return the id of the player whose turn it is 0,1 ..NUM_UNIS
int getWhoseTurn(Game game) {
  return (game->turn % NUM_UNIS + UNI_A);
}

// return the contents of the given vertex (ie campus code or
// VACENT_VERTEX)
int getCampus(Game g, path pathToVertex) {
#ifndef SUPERVISOR
  assert(validatePath(g->board, pathToVertex));
#endif
  return getVertexContents(g->board, pathToVertex);
}

// the contents of the given edge (ie ARC code or vacent ARC)
int getARC(Game g, path pathToEdge) {
#ifndef SUPERVISOR
  assert(validatePath(g->board, pathToEdge));
#endif
  return getEdgeContents(g->board, pathToEdge);
}

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player) {
  validatePlayer(player);
  return KPIpoints(getPlayer(g->players, player));
}

// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player) {
  validatePlayer(player);
  return ARCs(getPlayer(g->players, player));
}

// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player) {
  validatePlayer(player);
  return GO8s(getPlayer(g->players, player));
}

// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player) {
  validatePlayer(player);
  return campuses(getPlayer(g->players, player));
}

// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player) {
  validatePlayer(player);
  return IPs(getPlayer(g->players, player));
}

// return the number of Publications the specified player currently has
int getPublications (Game g, int player) {
  validatePlayer(player);
  return publications(getPlayer(g->players, player));
}

// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, int player, int discipline) {
  validatePlayer(player);
  validateDiscipline(discipline);
  return (students(getPlayer(g->players, player), discipline));
}


// make the specified action for the current player and update the
// game state accordingly.
// You may assume that the action is legal.
void makeAction (Game g, action a) {
  int code = a.actionCode;
  // whether this is the first time we are awarding arcs
  int firstArcAward = FALSE;
  // whether this is the first time we are awarding pubs
  int firstPubsAward = FALSE;
  Player currentPlayer = getPlayer(g->players, getWhoseTurn(g));

  if ((code == BUILD_CAMPUS) || (code == OBTAIN_ARC) ||
      (code == BUILD_GO8)) {
    validatePath(g->board, a.destination);
    build(g->board, a.destination, code, getWhoseTurn(g));
    deductStudents(currentPlayer, code);
    // once someone obtains an arc, start awarding
    // kpi points for obtaining arcs
    if (code == OBTAIN_ARC && !g->awardMostArcs) {
      g->awardMostArcs = TRUE;
      firstArcAward = TRUE;
    }
  } else if (code == START_SPINOFF) {
    deductStudents(currentPlayer, code);
  } else if (code == OBTAIN_PUBLICATION) {
    deductStudents(currentPlayer, code);
    givePublication(currentPlayer);
    // once someone obtains a publication, start awarding
    // kpi points for the most publications
    if (!g->awardMostPubs) {
      g->awardMostPubs = TRUE;
      firstPubsAward = TRUE;
    }
  } else if (code == OBTAIN_IP_PATENT) {
    deductStudents(currentPlayer, code);
    givePatent(currentPlayer);
  } else if (code == RETRAIN_STUDENTS) {
    validateDiscipline(a.disciplineFrom);
    validateDiscipline(a.disciplineTo);
    int exchangeRate = getExchangeRate (g, getWhoseTurn(g),
					a.disciplineFrom,
					a.disciplineTo);
    giveStudents(currentPlayer, a.disciplineFrom, - exchangeRate);
    giveStudents(currentPlayer, a.disciplineTo, 1);

  }
  if (g->awardMostArcs) {
    g->playerWithMostArcs = awardMostArcs(g->players,
					  g->playerWithMostArcs,
					  firstArcAward);
  }
  if (g->awardMostPubs) {
    g->playerWithMostPubs = awardMostPubs(g->players,
					  g->playerWithMostPubs,
					  firstPubsAward);
  }
}


// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player,
		     int disciplineFrom, int disciplineTo) {
  validatePlayer(player);
  int exchangeRate = 0;
  if ((disciplineFrom == STUDENT_MMONEY)
      && hasRetrainingCentre(g->board, player, TWO_TO_ONE_MMONEY)) {
    exchangeRate = 2;
  } else if ((disciplineFrom == STUDENT_MJ) &&
	     hasRetrainingCentre(g->board, player, TWO_TO_ONE_MJ)) {
    exchangeRate = 2;
  } else if ((disciplineFrom == STUDENT_MTV) &&
	     hasRetrainingCentre(g->board, player, TWO_TO_ONE_MTV)) {
    exchangeRate = 2;
  } else if ((disciplineFrom == STUDENT_BPS) &&
	     hasRetrainingCentre(g->board, player, TWO_TO_ONE_BPS)) {
    exchangeRate = 2;
  } else if ((disciplineFrom == STUDENT_BQN) &&
	     hasRetrainingCentre(g->board, player, TWO_TO_ONE_BQN)) {
    exchangeRate = 2;
  } else if ((disciplineFrom != STUDENT_THD) &&
	     hasRetrainingCentre(g->board, player,
				 THREE_TO_ONE_ANY)) {
    exchangeRate = 3;
  } else if (disciplineFrom != STUDENT_THD) {
    exchangeRate = 4; // anyone can always retrain 4 students
  }
  return exchangeRate;
}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started.
int getMostARCs (Game g) {
  return g->playerWithMostArcs;
}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first IP is patented after the game
// has started.
int getMostPublications (Game g) {
  return g->playerWithMostPubs;
}

// after the break we will talk about implementing this. for now
// you can have it doing nothing
void disposeGame (Game g) {
  disposePlayers(g->players);
  disposeBoard(g->board);
}


/************************ Board.c ************************************/

#define NUM_RETRAINING_VERTEXES 2
#define NUM_RETRAINING_CENTRES 9

#define NUM_START_POSITIONS 6

typedef struct retraining *RetrainingCentre;

struct board {
  Tile *tiles;
  Vertex startVertex;
  RetrainingCentre *retrainingCentres;
};

struct retraining {
  int centreType;
  Vertex vertexes[NUM_RETRAINING_VERTEXES];
};

//
Tile * getTiles(Board board) {
  return board->tiles;
}

// Creates retraining centres in the default positions
RetrainingCentre *createRetrainingCentres(Tile *tiles);

// initialises the starting positions
void createStartingPositions(Board board);

// Create a new board with given disciplines and dice
Board newBoard(int discipline[], int dice[]) {
  Tile *tiles = createTiles(discipline, dice);
  Board board = malloc(sizeof (struct board));
  board->tiles = tiles;
  board->startVertex = getStartVertex(tiles);
  board->retrainingCentres = createRetrainingCentres(tiles);
  createStartingPositions(board);
  return board;
}

int isValidPath(Board board, path destination) {
  int success = TRUE;
  getVertex(destination, board->startVertex, &success);
  return success;
}

// Creates retraining centres in the default positions
RetrainingCentre *createRetrainingCentres(Tile *tiles) {
  RetrainingCentre *centres = malloc(sizeof (RetrainingCentre)
				     * NUM_RETRAINING_CENTRES);
  int i = 0;
  // gives index of tile that ith retraining centre is attached to
  // (moving clockwise from the top left retraining centre)
  int rcTiles[] = {3, 12, 16, 17, 15, 11, 6, 1, 0};
  // gives vertexes (on the hex tile) that the
  // ith retraining centre is attached to
  int rcVertexOffset[] = {0, 0, 1, 2, 2, 3, 4, 4, 5};
  // gives the type of the ith retraining centre
  int rcTypes[] = {TWO_TO_ONE_MTV, TWO_TO_ONE_MMONEY, THREE_TO_ONE_ANY,
		   TWO_TO_ONE_BQN, TWO_TO_ONE_MJ, THREE_TO_ONE_ANY,
		   TWO_TO_ONE_BPS, THREE_TO_ONE_ANY, THREE_TO_ONE_ANY};

  while (i < NUM_RETRAINING_CENTRES) {
    int offset = rcVertexOffset[i];
    int tileNum = rcTiles[i];
    centres[i] = malloc (sizeof (struct retraining));
    Vertex *vertexes = getVertexes(tiles[tileNum]);
    centres[i]->vertexes[0] = vertexes[offset];
    centres[i]->vertexes[1] = vertexes[(offset + 1) % NUM_CORNERS];
    centres[i]->centreType = rcTypes[i];
    i++;
  }
  return centres;
}

// gets the vertex contents given a path string
int getVertexContents(Board board, path pathToVertex) {
  int success = TRUE;
  Vertex vertex = getVertex(pathToVertex, board->startVertex, &success);
  return getCampusType(vertex);
}

// gets the edge contents given a path string
int getEdgeContents(Board board, path pathToEdge) {
  int success = TRUE;
  Vertex vertex = getVertex(pathToEdge, board->startVertex, &success);
  return getArcType(vertex);
}

// returns whether the given player owns an arc adjacent to the
// destination vertex
int adjacentArcToVertex(Board board, path destination, int player) {
  int success = TRUE;
  Vertex vertex = getVertex(destination, board->startVertex, &success);
  return playerOwnsArc(vertex, player);
}

// returns whether the given player owns an arc adjacent to the
// destination edge
int adjacentArcToEdge(Board board, path destination, int player) {
  int success = TRUE;
  Edge edge = getEdge(destination, board->startVertex, &success);
  return hasAdjacentArc(edge, player);
}


// returns whether the given player owns a campus adjacent to the
// destination
int adjacentCampus(Board board, path destination, int player) {
  int success = TRUE;
  Vertex vertex = getVertex(destination, board->startVertex, &success);
  return hasAdjacentCampus(vertex, player);
}

int adjacentCampusToEdge(Board board, path destination, int player) {
  int success = TRUE;
  int hasAdjacentCampus = FALSE;
  Edge edge = getEdge(destination, board->startVertex, &success);
  if (getCampusType(getEdgeTo(edge)) == playerToGO8(player)
      || getCampusType(getEdgeTo(edge)) == playerToCampus(player)
      || getCampusType(getEdgeFrom(edge)) == playerToGO8(player)
      || getCampusType(getEdgeFrom(edge)) == playerToCampus(player)) {
    hasAdjacentCampus = TRUE;
  }
  return hasAdjacentCampus;
}

// returns whether *anyone* owns a campus adjacent to the destination
int adjacentCampuses(Board board, path destination) {
  return (adjacentCampus(board, destination, UNI_A)
	  || adjacentCampus(board, destination, UNI_B)
	  || adjacentCampus(board, destination, UNI_C));
}


// generate students for the given dice score
void generateStudents(Board board, Player players[], int diceScore) {
  Tile *tiles = board->tiles;
  int i, j;
  i = 0;
  while (i < NUM_REGIONS) {
    if (getDiceNumber(tiles[i]) == diceScore) {
      Vertex *vertexes = getVertexes(tiles[i]);
      j = 0;
      while (j < NUM_CORNERS) {
	int campusType = getCampusType(vertexes[j]);
	if (campusType != VACANT_VERTEX) {
	  int playerNum = campusToPlayer(campusType);
	  int numStudents = 1;
	  // GO8 campuses generate 2 students
	  if ((campusType == GO8_A) || (campusType == GO8_B)
	      || (campusType == GO8_C)) {
	    numStudents = 2;
	  }
	  giveStudents(getPlayer(players, playerNum),
		       getTileDiscipline(tiles[i]), numStudents);
	}
	j++;
      }
    }
    i++;
  }
}

// build the specified thing for the given player
void build(Board board, path destination,
	   int actionCode, int playerNum) {
  int success = TRUE;
  Vertex vertex = getVertex(destination, board->startVertex, &success);
  if (actionCode == BUILD_CAMPUS) {
    setCampusType(vertex, playerToCampus(playerNum));
  } else if (actionCode == BUILD_GO8) {
    setCampusType(vertex, playerToGO8(playerNum));
  } else if (actionCode == OBTAIN_ARC) {
    setArcType(vertex, playerToArc(playerNum));
  }
}

// return whether the given player has access to a retraining
// centre of the given type
int hasRetrainingCentre(Board board, int player, int centreType) {
  int i = 0;
  int hasCentre = FALSE;
  while (i < NUM_RETRAINING_CENTRES) {
    int j = 0;
    RetrainingCentre centre = board->retrainingCentres[i];
    if (centre->centreType == centreType) {
      while (j < NUM_RETRAINING_VERTEXES) {
	int campusType = getCampusType(centre->vertexes[j]);
	if (campusType == playerToCampus(player)
	    || campusType == playerToGO8(player)) {
	  hasCentre = TRUE;
	}
	j++;
      }
    }
    i++;
  }
  return hasCentre;
}

// initialises the starting positions of all the players on the board
// we traverse the board clockwise because the arc of
// each player faces to the right. so the same path can be used
// to specify the starting vertex and the arc.
void createStartingPositions(Board board) {
  // this gives the player id of the player who owns each campus/arc
  // in the order that we build them (i.e. traversing the board
  // around the outside clockwise)
  int playerIds[] = {UNI_C, UNI_B, UNI_A, UNI_C, UNI_B, UNI_A};

  // we start with the string "LRLRL",
  // appending "RRLRL" each time to get to the next starting vertex
  path destination = "LRLRL";

  int i = 0;
  while (i < NUM_START_POSITIONS) {
    build(board, destination, BUILD_CAMPUS, playerIds[i]);
    build(board, destination, OBTAIN_ARC, playerIds[i]);
    strcat(destination, "RRLRL");
    i++;
  }

}

void disposeBoard(Board board) {
  int i = 0;
  while (i < NUM_RETRAINING_CENTRES) {
    free(board->retrainingCentres[i]);
    i++;
  }
  disposeTiles(board->tiles);
  free(board);
}

/**************************** Tile.c **********************************/
// Make a new tile with given discipline and dice number
Tile newTile(int discipline, int diceNumber);

// gives tile above and to the left of the tile with coords
// row, col
Tile aboveAndLeft(Tile tiles[], int tilesPerColumn[], int row,
                  int column);

// gives tile below and to the left of the tile with coords
// row, col
Tile belowAndLeft(Tile tiles[], int tilesPerColumn[], int row,
		  int column);

// gets the tile from tiles array with coords row, col
Tile getTile(Tile tiles[], int tilesPerColumn[], int row, int col);

// create all vertexes of a given tile
void createVertexes(Tile *tiles, Tile tile, int row, int col,
		    int tileCount);

// create the top two corners (0 & 1) of a tile
void createTopCorners(Tile *tiles, Tile tile, int row, int col);

// link the top corners (0 & 1) of a tile to the tile above
void linkTopCorners(Tile *tiles, Tile tile, int tileCount);

// link vertexes 4 & 5 of the tile to the appropriate other tile
// in the tiles array
void linkBottomLeftCorners(Tile *tiles, Tile tile, int row, int col);

// connect all of the vertexes of a given tile
void connectVertexes(Tile tile);

struct tile {
  Vertex vertexes[NUM_CORNERS];
  int discipline;
  int diceNumber;
};

// connect all of the vertexes of a given tile
void connectVertexes(Tile tile) {
  connect(tile->vertexes[0], tile->vertexes[1], EW);
  connect(tile->vertexes[1], tile->vertexes[2], SE);
  connect(tile->vertexes[2], tile->vertexes[3], NE);
  connect(tile->vertexes[3], tile->vertexes[4], EW);
  connect(tile->vertexes[4], tile->vertexes[5], SE);
  connect(tile->vertexes[5], tile->vertexes[0], NE);
}

// Gets the starting vertex for paths (as described in the project spec)
Vertex getStartVertex(Tile *tiles) {
  return tiles[START_TILE]->vertexes[START_VERTEX];
}

// Returns the discipline of students that the tile generates
int getTileDiscipline(Tile tile) {
  return tile->discipline;
}

// Returns the dice number of the tile
int getDiceNumber(Tile tile) {
  return tile->diceNumber;
}

// Gets all vertexes of the tile as an array
Vertex *getVertexes(Tile tile) {
  return tile->vertexes;
}

// creates the board
Tile *createTiles(int discipline[], int dice[]) {
  Tile *tiles = malloc(sizeof(struct tile) * NUM_REGIONS);
  int tilesPerColumn[NUM_COLUMNS] = {3, 4, 5, 4, 3};
  int col = 0;
  int row;
  int tileCount = 0;
  while (col < NUM_COLUMNS) {
    row = 0;
    while (row < tilesPerColumn[col]) {
      Tile tile = newTile(discipline[tileCount], dice[tileCount]);
      tiles[tileCount] = tile;
      createVertexes(tiles, tile, row, col, tileCount);
      connectVertexes(tile);
      row++;
      tileCount++;
    }
    col++;
  }
  return tiles;
}

void createVertexes(Tile *tiles, Tile tile, int row, int col,
		    int tileCount) {
  // create top two corners (vertices 0 & 1)
  // if in first row, otherwise link to previously
  // created ones
  if (row == 0) {
    createTopCorners(tiles, tile, row, col);
  } else {
    linkTopCorners(tiles, tile, tileCount);
  }

  // always create new 2nd & 3rd vertices
  tile->vertexes[2] = newVertex();
  tile->vertexes[3] = newVertex();

  // create vertices 4 & 5 if in first column,
  // otherwise link to previously created ones
  if (col == 0) {
    tile->vertexes[4] = newVertex();
    tile->vertexes[5] = newVertex();
  } else {
    linkBottomLeftCorners(tiles, tile, row, col);
  }
}

void createTopCorners(Tile *tiles, Tile tile, int row, int col) {
  // if in the first half, create new top left vertex
  // otherwise link to the second vertex of tile in previous column
  Tile linkto;
  int tilesPerColumn[NUM_COLUMNS] = {3, 4, 5, 4, 3};
  if (col <= (NUM_COLUMNS / 2)) {
    tile->vertexes[0] = newVertex();
  } else {
    linkto = aboveAndLeft(tiles, tilesPerColumn,
			  row, col);
    tile->vertexes[0] = linkto->vertexes[2];
  }
  tile->vertexes[1] = newVertex();
}

void linkTopCorners(Tile *tiles, Tile tile, int tileCount) {
  // link top corners to bottom of tile above
   tile->vertexes[0]
     = tiles[tileCount - 1]->vertexes[4];
   tile->vertexes[1]
     = tiles[tileCount - 1]->vertexes[3];
}

void linkBottomLeftCorners(Tile *tiles, Tile tile, int row, int col) {
  int tilesPerColumn[NUM_COLUMNS] = {3, 4, 5, 4, 3};
  Tile linkto;
  // 4th is linked to 2nd of hexagon below and to the left
  // (except for last row in first half)
  if (!((row == tilesPerColumn[col] - 1)
	&& (col <= (NUM_COLUMNS / 2)))) {
    linkto = belowAndLeft(tiles, tilesPerColumn,
			  row, col);
    tile->vertexes[4] = linkto->vertexes[2];
  } else {
    tile->vertexes[4] = newVertex();
  }
  // 5th is linked to 3rd hexagon of above and to the left
  // (except for first row in first half)
  if (!(row == 0 && (col <= (NUM_COLUMNS / 2)))) {
    linkto = aboveAndLeft(tiles, tilesPerColumn,
			  row, col);
    tile->vertexes[5] = linkto->vertexes[3];
  } else {
    linkto = belowAndLeft(tiles, tilesPerColumn,
			  row, col);
    tile->vertexes[5] = linkto->vertexes[1];
  }
}

// Make a new tile with given discipline and dice number
Tile newTile(int discipline, int diceNumber) {
  Tile tile = malloc (sizeof (struct tile));
  tile->discipline = discipline;
  tile->diceNumber = diceNumber;
  return tile;
}

// gives tile above and to the left of the tile with index
// tileCount
Tile aboveAndLeft(Tile tiles[], int tilesPerColumn[], int row,
                  int column) {
  int firstHalf = (column <= (NUM_COLUMNS / 2));
  int desiredRow, desiredCol;
  if (firstHalf) {
    desiredRow = row - 1;
  } else {
    desiredRow = row;
  }
  desiredCol = column - 1;
  return (getTile(tiles, tilesPerColumn, desiredRow, desiredCol));
}

// gives tile below and to the left of tile with index
// tileCount
Tile belowAndLeft(Tile tiles[], int tilesPerColumn[], int row,
		  int column) {
  int firstHalf = (column <= (NUM_COLUMNS / 2));
  int desiredRow, desiredCol;
  if (firstHalf) {
    desiredRow = row;
  } else {
    desiredRow = row + 1;
  }
  desiredCol = column - 1;
  return (getTile(tiles, tilesPerColumn, desiredRow, desiredCol));
}

// gets the tile at row, col from the tiles array
Tile getTile(Tile tiles[], int tilesPerColumn[], int row, int col) {
  int tileCount = 0;
  int i = 0;
  while (i < col) {
    tileCount += tilesPerColumn[i];
    i++;
  }
  i = 0;
  while (i < row) {
    tileCount++;
    i++;
  }
  return tiles[tileCount];
}

void disposeTiles(Tile tiles[]) {
  int i = 0;
  while (i < NUM_REGIONS) {
    // TODO disposing vertexes
    free(tiles[i]);
    i++;
  }
}

/*************************** Vertex.c *********************************/

struct vertex {
  Edge edges[NUM_EDGES];
  Edge requestedEdge;
  int numConnections;
  int campusType;
};

struct edge {
  Vertex from;
  Vertex to;
  Orientation orientation;
  int arcType;
};

Vertex newVertex(void) {
  Vertex vertex = malloc(sizeof (struct vertex));
  vertex->numConnections = 0;
  vertex->campusType = VACANT_VERTEX;
  vertex->requestedEdge = NULL;
  return vertex;
}

// get the campus type that the vertex is holding
int getCampusType(Vertex vertex) {
  return vertex->campusType;
}

// set the campus type of the given vertex to the given value
void setCampusType(Vertex vertex, int campusType) {
  vertex->campusType = campusType;
}

// get the arc type of the vertex's requested edge (if it was the
// last vertex in a specified path)
int getArcType(Vertex vertex) {
  return vertex->requestedEdge->arcType;
}

// set the arc type of the given vertex's requested edge to the given
// value
void setArcType(Vertex vertex, int arcType) {
  vertex->requestedEdge->arcType = arcType;
}

static void connect(Vertex a, Vertex b, int direction) {
  // can never have more than 3 connections
  // per vertex
  if (!connected(a, b)) {
    Edge edge = newEdge(a, b, direction);
    a->edges[a->numConnections] = edge;
    b->edges[b->numConnections] = edge;
    a->numConnections++;
    b->numConnections++;
  }
}

Edge newEdge(Vertex to, Vertex from, int direction) {
  Edge edge = malloc(sizeof(struct edge));
  edge->from = from;
  edge->to = to;
  edge->orientation = direction;
  edge->arcType = VACANT_ARC;
  return edge;
}

int connected(Vertex a, Vertex b) {
  int i = 0;
  int connectionExists = FALSE;
  while (i < a->numConnections) {
    if ( ((a->edges[i]->from == a) && (a->edges[i]->to == b))
	 || ((a->edges[i]->from == b) && (a->edges[i]->to == a))) {
      connectionExists = TRUE;
    }
    i++;
  }
  return connectionExists;
}

// gets a vertex given the path.
// saves the last edge in the path as the vertex's
// 'requested edge' field.
Vertex getVertex(path pathToDest, Vertex start, int *success) {
  Vertex currentVertex = start;
  Edge currentEdge = currentVertex->edges[0];
  Vertex nextVertex = NULL;
  int nextMove = UNDEFINED;
  // defined as part of the project spec.
  // first move is made facing with back to sea.
  int lastMove = SE;
  int i = 0;
  while (i < strlen(pathToDest) && *success) {
    // work out the direction which we're going to be travelling
    nextMove = getNextMove(pathToDest[i], lastMove);
    // find the which vertex we're going to be travelling to
    nextVertex = getNextVertex(currentVertex, nextMove, success);
    if (*success) {
      // find the edge that we're going to be travelling on
      currentEdge = getEdgeWithOrientation(nextVertex, nextMove, success);
    }
    // go there
    lastMove = nextMove;
    currentVertex = nextVertex;
    i++;
  }
  if (*success) {
    // save the last edge in the path as the "requested edge"
    // field in the last vertex of the path
    currentVertex->requestedEdge = currentEdge;
  }
  return currentVertex;
}

Edge getEdge(path destination, Vertex start, int *success) {
  return getVertex(destination, start, success)->requestedEdge;
}

Vertex getEdgeTo(Edge e) {
  return e->to;
}

Vertex getEdgeFrom(Edge e) {
  return e->from;
}

Vertex getNextVertex(Vertex currentVertex, int nextMove, int *success) {
  Vertex nextVertex = NULL;
  int i = 0;
  while (i < currentVertex->numConnections) {
    if (currentVertex->edges[i]->orientation == nextMove) {
      if (currentVertex->edges[i]->from == currentVertex) {
	nextVertex = currentVertex->edges[i]->to;
      } else {
	nextVertex = currentVertex->edges[i]->from;
      }
    }
    i++;
  }
  if (nextVertex == NULL) {
    *success = FALSE;
  }
  return nextVertex;
}

Edge getEdgeWithOrientation(Vertex nextVertex, int nextMove, int *success) {
  Edge desiredEdge = NULL;
  int i = 0;
  while (i < nextVertex->numConnections) {
    if (nextVertex->edges[i]->orientation == nextMove) {
      desiredEdge = nextVertex->edges[i];
    }
    i++;
  }
  // we shouldn't request an edge that doesn't exist
  if (desiredEdge == NULL) {
    *success = FALSE;
  }
  return desiredEdge;
}

int getNextMove(char direction, int lastMove) {
  // leftTurns[OLD_DIRECTION] will give the new direction
  // that you end up facing when you turn left after coming
  // from OLD_DIRECTION
  // e.g. leftTurns[SE] will give EW
  // EW, SE, NE
  int nextMove = UNDEFINED;
  int leftTurns[] = {NE, EW, SE};
  int rightTurns[] = {SE, NE, EW};
  if (direction == 'B') {
    nextMove = lastMove;
  } else if (direction == 'R') {
    nextMove = rightTurns[lastMove];
  } else if (direction == 'L') {
    nextMove = leftTurns[lastMove];
  }
  return nextMove;
}


// returns whether the player owns an arc attached to that vertex
int playerOwnsArc(Vertex vertex, int player) {
  int playerArcType = playerToArc(player);
  int i = 0;
  int ownsArc = FALSE;
  while (i < vertex->numConnections) {
    if (vertex->edges[i]->arcType == playerArcType) {
      ownsArc = TRUE;
    }
    i++;
  }
  return ownsArc;
}

// return whether a player has an arc attached to the given edge
int hasAdjacentArc(Edge edge, int player) {
  return (playerOwnsArc(edge->to, player)
	  || playerOwnsArc(edge->from, player));
}

int hasAdjacentCampus(Vertex vertex, int player) {
  int playerUniType = playerToCampus(player);
  int playerGO8Type = playerToGO8(player);
  int i = 0;
  int playerOwnsAdjacentCampus = FALSE;
  while (i < vertex->numConnections) {
    Edge edge = vertex->edges[i];
    if (edge->to->campusType == playerUniType
           || edge->to->campusType == playerGO8Type
           || edge->from->campusType == playerUniType
	|| edge->from->campusType == playerGO8Type) {
      playerOwnsAdjacentCampus = TRUE;
    }
    i++;
  }
  return playerOwnsAdjacentCampus;
}

void disposeVertex(Vertex vertex) {
  // TODO
}

/*************************** Player.c *********************************/
Player newPlayer(int playerNum) {
  Player player = malloc(sizeof(struct player));
  player->playerNum = playerNum;
   player->kpiPoints = INITIAL_NUM_CAMPUSES * CAMPUS_POINTS
     + INITIAL_NUM_ARCS * ARC_POINTS;
   player->numArcs = INITIAL_NUM_ARCS;
   player->numGO8s = 0;
   player->numCampuses = INITIAL_NUM_CAMPUSES;
   player->numIPs = 0;
   player->numPubs = 0;
   int i = 0;
   while (i < NUM_DISCIPLINES) {
     player->students[i] = 0;
     i++;
   }
   player->numUnusedSpinoffs = 0;
   return player;
}

// return the number of KPI points the specified player currently has
int KPIpoints (Player player) {
  return player->kpiPoints;
}

// return the number of ARC grants the specified player currently has
int ARCs (Player player) {
  return player->numArcs;
}

// return the number of GO8 campuses the specified player currently has
int GO8s (Player player) {
  return player->numGO8s;
}

// return the number of normal Campuses the specified player currently has
int campuses (Player player) {
  return player->numCampuses;
}

// return the number of IP Patents the specified player currently has
int IPs (Player player) {
  return player->numIPs;
}

// return the number of Publications the specified player currently has
int publications (Player player) {
  return player->numPubs;
}

// return the number of students of the specified discipline type
// the specified player currently has
int students (Player player, int discipline) {
  return player->students[discipline];
}

// returns whether the player has enough students to
// perform the specified action
int enoughStudents(Player player, int actionCode) {
  int enough = TRUE;
  int i = 0;
  while (i < NUM_DISCIPLINES) {
    if (player->students[i] < howManyStudents(actionCode, i)) {
      enough = FALSE;
    }
    i++;
  }
  return enough;
}

// give the player the specified number of students of the given
// discipline
void giveStudents(Player player, int discipline, int numStudents) {
  player->students[discipline] += numStudents;
}

// deduct the player the number of students for the required
// action code. also award any kpi points/spinoffs
// to the given player for the given action.
void deductStudents (Player player, int actionCode) {
  int i = 0;
  int pointsAwarded = 0;
  while (i < NUM_DISCIPLINES) {
    player->students[i] -= howManyStudents(actionCode, i);
    i++;
  }
  if (actionCode == BUILD_CAMPUS) {
    pointsAwarded = CAMPUS_POINTS;
    player->numCampuses++;
  } else if (actionCode == BUILD_GO8) {
    pointsAwarded = GO8_POINTS;
    player->numCampuses--;
    player->numGO8s++;
  } else if (actionCode == OBTAIN_ARC) {
    pointsAwarded = ARC_POINTS;
    player->numArcs++;
  } else if (actionCode == START_SPINOFF) {
    player->numUnusedSpinoffs++;
  }
  player->kpiPoints += pointsAwarded;
}

// returns how many students of the given discipline
// are required for the given action
// Order of arrays is order of #define's i.e.
// STUDENT_THD, STUDENT_BPS, STUDENT_BQN,
// STUDENT_MJ, STUDENT_MTV, STUDENT_MMONEY
int howManyStudents(int actionCode, int discipline) {
  int buildCampus[NUM_DISCIPLINES] = {0, 1, 1, 1, 1, 0};
  int buildGO8[NUM_DISCIPLINES] = {0, 0, 0, 2, 0, 3};
  int obtainArc[NUM_DISCIPLINES] = {0, 1, 1, 0, 0, 0};
  int startSpinoff[NUM_DISCIPLINES] = {0, 0, 0, 1, 1, 1};
  int howMany = 0;
  if (actionCode == BUILD_CAMPUS) {
    howMany = buildCampus[discipline];
  } else if (actionCode == BUILD_GO8) {
    howMany = buildGO8[discipline];
  } else if (actionCode == OBTAIN_ARC) {
    howMany = obtainArc[discipline];
  } else if (actionCode == OBTAIN_PUBLICATION
             || actionCode == OBTAIN_IP_PATENT
             || actionCode == START_SPINOFF) {
    howMany = startSpinoff[discipline];
  }
  return howMany;
}

// give the player a publication
void givePublication(Player player) {
  player->numPubs++;
}

// give the player a patent
void givePatent(Player player) {
  player->numIPs++;
  player->kpiPoints += PATENT_POINTS;
}

// convert MTV & MMONEY students of all players to THDs
void convertStudentsToTHDs(Player *players) {
  int i = 0;
  while (i < NUM_UNIS) {
    int j = 0;
    int totalStudents = 0;
    while (j < NUM_DISCIPLINES) {
      if ((j == STUDENT_MTV) || j == (STUDENT_MMONEY)) {
	totalStudents += players[i]->students[j];
	players[i]->students[j] = 0;
      }
      j++;
    }
    players[i]->students[STUDENT_THD] += totalStudents;
    i++;
  }
}

// awards the points for most arc grants. returns the number of the
// new player with the most arc grants
int awardMostArcs(Player *players, int playerWithMostArcs,
		  int firstArcAward) {
  int mostArcs;
  int newPlayerWithMostArcs = NO_ONE;
  if (firstArcAward) {
    mostArcs = 0;
  } else {
    mostArcs = getPlayer(players, playerWithMostArcs)->numArcs;
    newPlayerWithMostArcs = playerWithMostArcs;
  }
  int i = 0;
  while (i < NUM_UNIS) {
    if (players[i]->numArcs > mostArcs) {
      mostArcs = players[i]->numArcs;
      newPlayerWithMostArcs = i + UNI_A;
    }
    i++;
  }
  if (!firstArcAward) {
    getPlayer(players, playerWithMostArcs)->kpiPoints -= MOST_ARC_POINTS;
  }
  getPlayer(players, newPlayerWithMostArcs)->kpiPoints += MOST_ARC_POINTS;
  return newPlayerWithMostArcs;
}

// awards the points for most publications. returns the number of the
// new player with the most publications

// lots of duplication with the arc functionality - could merge these
// into one 'prestige' function
int awardMostPubs(Player *players, int playerWithMostPubs,
		  int firstPubsAward) {
  int mostPubs;
  int newPlayerWithMostPubs = NO_ONE;
  if (firstPubsAward) {
    mostPubs = 0;
  } else {
    mostPubs = getPlayer(players, playerWithMostPubs)->numPubs;
    newPlayerWithMostPubs = playerWithMostPubs;
  }
  int i = 0;
  while (i < NUM_UNIS) {
    if (players[i]->numPubs > mostPubs) {
      mostPubs = players[i]->numPubs;
      newPlayerWithMostPubs = i + UNI_A;
    }
    i++;
  }
  if (!firstPubsAward) {
    getPlayer(players, playerWithMostPubs)->kpiPoints -= MOST_PUBS_POINTS;
  }
  getPlayer(players, newPlayerWithMostPubs)->kpiPoints += MOST_PUBS_POINTS;
  return newPlayerWithMostPubs;
}

// get the total number of GO8s owned by all the players
int totalGO8s(Player *players) {
  int i = 0;
  int total = 0;
  while (i < NUM_UNIS) {
    total += players[i]->numGO8s;
    i++;
  }
  return total;
}

// get the total number of unused spinoffs a player has
int getUnusedSpinoffs(Player player) {
  return player->numUnusedSpinoffs;
}

// convert from player number to relevant campus code
int playerToCampus(int player) {
  int campusCodes[NUM_UNIS] = {CAMPUS_A, CAMPUS_B, CAMPUS_C};
  return campusCodes[player - UNI_A];
}

// convert from campus code to player number
int campusToPlayer(int campus) {
  int playerCodes[NUM_UNIS*2] = {UNI_A, UNI_B, UNI_C, UNI_A, UNI_B, UNI_C};
  return playerCodes[campus - CAMPUS_A];
}

// convert from player number to relevant GO8 code
int playerToGO8(int player) {
  int GO8Codes[NUM_UNIS] = {GO8_A, GO8_B, GO8_C};
  return GO8Codes[player - UNI_A];
}

// convert from player number to relevant arc code
int playerToArc(int player) {
  int arcCodes[NUM_UNIS] = {ARC_A, ARC_B, ARC_C};
  return arcCodes[player - UNI_A];
}

// get the player with number playerNum from the players array
Player getPlayer(Player players[], int playerNum) {
  return players[playerNum - UNI_A];
}

void disposePlayers(Player players[]) {
  int i = 0;
  while (i < NUM_UNIS) {
    free(players[i]);
    i++;
  }
}
