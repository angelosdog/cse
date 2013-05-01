//
//  testGame.c
//  Edited by Vincent Tran 22/05/12
//
//  Created by Jiashu Chen on 24/04/12.
//  Copyright (c) 2012 University Of New South Wales. All rights reserved.
//

/*
 *  testGame.c
 *  1.0
 *  1917-w123
 *
 *  Created by Richard Buckland on 28/04/11.
 *  Copyright 2011 Licensed under Creative Commons SA-BY-NC 3.0. 
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "Game.h"

//paths to retraining centre
#define MMONEY_RETRAINING "LR"
#define MTV_RETRAINING "RR"
#define BPS_RETRAINING "RRLRLLRLRLLR"
#define BQN_RETRAINING "LRLRLRRLRLBR"
#define MJ_RETRAINING "RLRLRLRLRLLLR"
#define SECOND_MMONEY_RETRAINING "LRL"
#define SECOND_MJ_RETRAINING "RLRLRLRLRLLLRL"

#define A_PATH1 "RR"
#define A_PATH2 "RLRLRLRLRLLLL"
#define A_PATH3 "RLRLRLRLRLLLLR"
#define A_PATH4 "RLRLRLRLRLLLLRL"
#define A_PATH5 "RLRLRLRLRLLL"
#define A_PATH6 "RLRLRLRLRLLLLR"
#define A_PATH7 "RLRLRLRLRLLLLL"

#define A_STARTING1 "RB"
#define A_STARTING2 "RLRLRLRLRLLB"

#define B_PATH1 "LRLRLRRLRLBR"
#define B_PATH2 "RRLRLLL"
#define B_PATH3 "LRLRLRRLRLBL"

#define B_STARTING1 "RRLRLLB"
#define B_STARTING2 "LRLRLRRLRL"

#define C_PATH1 "LRLRLBR"
#define C_PATH2 "LRLRLBL"
#define C_PATH3 "LRLRLBLR"
#define C_PATH4 "LRLRLBLRL"

#define C_STARTING1 "LRLRLB"
#define C_STARTING2 "RRLRLLRLRLLB"

// scores for game
#define KPI_OBTAIN_ARC 2
#define KPI_MOST_ARCS 10
#define KPI_OBTAIN_IP_PATENT_PATENT 10



#define DEFAULT_DISCIPLINES {5,3,3,2,4,3,1,1,4,0,2,1,5,3,5,4,2,2,4}
#define DEFAULT_DICE {6,10,8,3,4,9,6,11,11,7,12,3,5,2,5,10,9,4,8}
#define TOTAL_NUMBER_OF_ACTIONS 8
//#define NO_NEW_STUDENTS 

#define TOTAL_TURN_TESTS 7

void testNewGame(void);
void testDefaultValues(Game g, int disciplines[], int diceValues[]);

void testTurns (void);

void testThrowDice(void);
void testGetMost (void);

void testExchangeRate(void);
void testIsLegalAction(void);

int main (int argc, const char * argv[]) {
    testTurns ();
    testNewGame();
    testThrowDice();
    testExchangeRate();
    testGetMost();
    testIsLegalAction();

    printf ("All tests passed!  you are Awesome\n");
    return EXIT_SUCCESS;
}

void testNewGame(void) {
    printf ("Testing new game with sample board\n");
    // check makes a game without crashing
    int disciplines[NUM_REGIONS] = DEFAULT_DISCIPLINES;
    int diceValues [NUM_REGIONS] = DEFAULT_DICE;
    
    Game g = newGame (disciplines, diceValues);
    testDefaultValues(g, disciplines, diceValues);
    disposeGame (g);
    printf ("Passed initialisation tests\n\n");
}

void testDefaultValues(Game g, int disciplines[], int diceValues[]){
    printf("Initial turn number should be -1. Returned %d\n", getTurnNumber(g));
    assert (getTurnNumber(g) == -1);

    printf("The current player's turn should be NO_ONE. Returned %d.\n", getWhoseTurn(g));
    assert (getWhoseTurn(g) == 0);
   
    int i;
    printf("Checking the board has been initialised correctly\n");
    
for (i = 0; i < NUM_REGIONS; i++){
        printf("The discipline for region %d is %d and the dice value is %d. Returned %d and %d respctively\n",
               i, disciplines[i], diceValues[i], getDiscipline(g, i), getDiceValue(g, i));
        assert(disciplines[i] == getDiscipline(g,i) && diceValues[i] == getDiceValue(g,i));
    }
    
    printf("Checking initial score values\n");
    for (i = UNI_A; i <= UNI_C; i++){
        printf("Player %d should have the following scores:\n", i);

        printf("KPIs: 24. Returned %d.\n", getKPIpoints(g,i));
        assert(getKPIpoints(g,i) == 24);

        printf("Campuses: 2. Returned %d.\n", getCampuses(g,i));
        assert(getCampuses(g,i) == 2);

        printf("ARC Grants: 2. Returned %d.\n", getARCs(g,i));
        assert(getARCs(g,i) == 2);

        printf("GO8s: 0. Returned %d.\n", getGO8s(g,i));
        assert(getGO8s(g, i) == 0);

        printf("IPs: 0. Returned %d.\n", getIPs(g,i));
        assert(getIPs(g,i) == 0);

        printf("Publications 0. Returned %d.\n", getPublications(g,i));
        assert(getPublications(g,i) == 0);

        printf("THDs: 0. Returned %d.\n", getStudents(g,i,STUDENT_THD));
        assert(getStudents(g,i,STUDENT_THD) == 0);

        printf("BPs: 0. Returned %d.\n", getStudents(g,i,STUDENT_BPS));
        assert(getStudents(g,i,STUDENT_BPS) == 0);

        printf("BQNs: 0. Returned %d.\n", getStudents(g,i,STUDENT_BQN));
        assert(getStudents(g,i,STUDENT_BQN) == 0);

        printf("MJs: 0. Returned %d.\n", getStudents(g,i,STUDENT_MJ));
        assert(getStudents(g,i,STUDENT_MJ) == 0);

        printf("MTV: 0. Returned %d.\n", getStudents(g,i,STUDENT_MTV));
        assert(getStudents(g,i,STUDENT_MTV) == 0);

        printf("MMONEY: 0. Returned %d.\n", getStudents(g,i,STUDENT_MMONEY));
        assert(getStudents(g,i,STUDENT_MMONEY) == 0);
    } 
    printf("Checking default ARC grants\n");
    printf("Checking UNI_A's starting ARC at destination 'R'\n");
    assert(getARC(g, "R") == UNI_A);
    printf("Checking UNI_A's starting ARC at destination 'RLRLRLRLRLLL'\n");
    assert(getARC(g, "RLRLRLRLRLLL") == UNI_A);
    printf("Checking UNI_B's starting ARC at destination 'RRLRLL'\n");
    assert(getARC(g, "RRLRLL") == UNI_B);
    printf("Checking UNI_B's starting ARC at destination 'LRLRLRRLRLB'\n");
    assert(getARC(g, "LRLRLRRLRLB") == UNI_B);
    printf("Checking UNI_C's starting ARC at destination 'LRLRLB'\n");
    assert(getARC(g, "LRLRLB") == UNI_C);
    printf("Checking UNI_C's starting ARC at destination 'RRLRLLRLRLL'\n");
    assert(getARC(g, "RRLRLLRLRLL") == UNI_C);

    printf("Checking default campuses\n");
    printf("Checking UNI_A's starting campus at destination 'RB'\n");
    assert(getARC(g, "RB") == UNI_A);
    printf("Checking UNI_A's starting campus at destination 'RLRLRLRLRLLB'\n");
    assert(getARC(g, "RLRLRLRLRLLLB") == UNI_A);
    printf("Checking UNI_B's starting campus at destination 'RRLRLLB'\n");
    assert(getARC(g, "RRLRLLB") == UNI_B);
    printf("Checking UNI_B's starting campus at destination 'LRLRLRRLRL'\n");
    assert(getARC(g, "LRLRLRRLRL") == UNI_B);
    printf("Checking UNI_C's starting campus at destination 'LRLRLB'\n");
    assert(getARC(g, "LRLRLB") == UNI_C);
    printf("Checking UNI_C's starting campus at destination 'RRLRLLRLRLLB'\n");
    assert(getARC(g, "RRLRLLRLRLLB") == UNI_C);  
}

void testTurns (void) {
    printf ("Testing turn related functions\n");
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    int currentTurn;
    int previousTurn = -1;
    int turn;
    Game g;
    g = newGame (disciplines, dice);
    printf("Initial turn number should be -1. Returned %d.\n", getTurnNumber(g));
    assert(getTurnNumber(g) == -1);
    printf("Test correct if turn number and whose turn are correct from turns -1 to %d\n", TOTAL_TURN_TESTS);
    for(turn = -1; turn < TOTAL_TURN_TESTS; turn++){
        printf("It should be player %d's turn. Returned %d\n", turn%3+1, getWhoseTurn(g));
        assert((turn%3+1) == getWhoseTurn(g));
        throwDice(g,7);
        currentTurn = getTurnNumber(g);
        printf("Checking if turn number has incremented by 1 after throwing dice\n");
        assert(currentTurn == previousTurn + 1);
        previousTurn = currentTurn;
    }
    disposeGame (g);

    printf ("Passed all turn related tests\n\n");
}

void testThrowDice(void){
    //No need to test if the turn number increments, I've already done that.
    int disciplines[] = DEFAULT_DISCIPLINES;
    // For the purposes of testing
    disciplines[11] = STUDENT_THD;
    int dice[] = DEFAULT_DICE;
    dice[2] = 5;
    Game g;
    action a;
    g = newGame (disciplines, dice);
    int previousStudent = 0;
    int currentStudent;

    printf("Throwing an 11 gives UNI_A a BPS\n");
    throwDice(g, 11);
    currentStudent = getStudents(g, UNI_A, STUDENT_BPS);
    printf("UNI_A should now have 1 BPs. Returned %d.\n", currentStudent);
    assert(currentStudent == previousStudent+1);
    previousStudent = 0;

    printf("Throwing an 3 gives UNI_A a THD\n");
    throwDice(g, 3);
    currentStudent = getStudents(g, UNI_A, STUDENT_THD);
    printf("UNI_A should now have 1 THD. Returned %d.\n", currentStudent);
    assert(currentStudent == previousStudent+1);
    previousStudent = 0;

    printf("Throwing an 6 gives UNI_B a MMONEY\n");
    throwDice(g, 6);
    currentStudent = getStudents(g, UNI_B, STUDENT_MMONEY);
    printf("UNI_B should now have 1 MMONEY. Returned %d.\n", currentStudent);
    assert(currentStudent == previousStudent+1);
    previousStudent = 0;

    printf("Throwing an 8 gives UNI_B an MTV\n");
    throwDice(g, 8);
    currentStudent = getStudents(g, UNI_B, STUDENT_MTV);
    printf("UNI_B should now have 1 MTV. Returned %d.\n", currentStudent);
    assert(currentStudent == previousStudent+1);
    previousStudent = 0;

    printf("Throwing an 5 gives UNI_C an MJ\n");
    throwDice(g, 5);
    currentStudent = getStudents(g, UNI_C, STUDENT_MJ);
    printf("UNI_C should now have 1 MJs. Returned %d.\n", currentStudent);
    assert(currentStudent == previousStudent+1);
    previousStudent = 0;

    printf("Throwing an 9 gives UNI_C a BQN\n");
    throwDice(g, 9);
    currentStudent = getStudents(g, UNI_C, STUDENT_BQN);
    printf("UNI_C should now have 1 BQN. Returned %d.\n", currentStudent);
    assert(currentStudent == previousStudent+1);
    previousStudent = 0;

    printf("Testing effects of rolling a 7. All 3 uni's should have 0 MTVs and 0 MMONEYs.\n");
    throwDice(g, 7);
    printf("UNI_A now has %d MTVs and %d MMONEYS.\n", getStudents(g, UNI_A, STUDENT_MTV), getStudents(g, UNI_A, STUDENT_MMONEY));
    assert(getStudents(g, UNI_A, STUDENT_MTV) == 0 && getStudents(g, UNI_A, STUDENT_MMONEY) == 0);

    printf("UNI_B now has %d MTVs and %d MMONEYS.\n", getStudents(g, UNI_B, STUDENT_MTV), getStudents(g, UNI_B, STUDENT_MMONEY));
    assert(getStudents(g, UNI_B, STUDENT_MTV) == 0 && getStudents(g, UNI_B, STUDENT_MMONEY) == 0);

    printf("UNI_A now has %d MTVs and %d MMONEYS.\n", getStudents(g, UNI_C, STUDENT_MTV), getStudents(g, UNI_C, STUDENT_MMONEY));
    assert(getStudents(g, UNI_C, STUDENT_MTV) == 0 && getStudents(g, UNI_C, STUDENT_MMONEY) == 0);
    printf("\n");
    disposeGame(g);

    printf("Testing resource returns on GO8s\n");
    printf("Collected resources for all three players to build 2 GO8s\n");

    // Simulate a board
    disciplines[0] = STUDENT_MMONEY;
    disciplines[2] = STUDENT_MMONEY;
    disciplines[7] = STUDENT_MMONEY;
    disciplines[11] = STUDENT_MJ;
    disciplines[16] = STUDENT_MJ;
    disciplines[18] = STUDENT_MJ;
    dice[0] = 5;
    dice[2] = 5;
    dice[7] = 5;
    dice[11] = 5;
    dice[16] = 5;
    dice[18] = 5;
    g = newGame (disciplines, dice);
    currentStudent = 0;
    throwDice(g, 5);
    throwDice(g, 5);
    throwDice(g, 5);
    throwDice(g, 5);
    throwDice(g, 5);
    throwDice(g, 5);   

    printf("UNI_A should have 6 MJs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MJ));
    assert(getStudents(g, UNI_A, STUDENT_MJ) == 6);
    printf("UNI_A should have 6 MMONEYs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MMONEY));
    assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 6);
    printf("UNI_B should have 6 MJs. Returned %d.\n", getStudents(g, UNI_B, STUDENT_MJ));
    assert(getStudents(g, UNI_B, STUDENT_MJ) == 6);
    printf("UNI_B should have 6 MMONEYs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MMONEY));
    assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 6);
    printf("UNI_C should have 6 MJs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MJ));
    assert(getStudents(g, UNI_C, STUDENT_MJ) == 6);
    printf("UNI_C should have 6 MMONEYs. Returned %d.\n\n", getStudents(g, UNI_A, STUDENT_MMONEY));
    assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 6);

    a.actionCode = BUILD_GO8;
    int playersTested = 0;
    while (playersTested < 3) {
        if (getWhoseTurn(g) == UNI_A) {
            strcpy(a.destination, A_STARTING1);
            makeAction(g, a);
            printf("UNI_A built a GO8 at '%s'.\n", A_STARTING1);
            strcpy(a.destination, A_STARTING2);
            makeAction(g, a);
            printf("UNI_A built a GO8 at '%s'.\n", A_STARTING2);
        } else if (getWhoseTurn(g) == UNI_B) {
            strcpy(a.destination, B_STARTING1);
            makeAction(g, a);
            printf("UNI_B built a GO8 at '%s'.\n", B_STARTING1);
            strcpy(a.destination, B_STARTING2);
            makeAction(g, a);
            printf("UNI_B built a GO8 at '%s'.\n", B_STARTING2);
        } else if (getWhoseTurn(g) == UNI_C){  
            strcpy(a.destination, C_STARTING1);
            makeAction(g, a);
            printf("UNI_C built a GO8 at '%s'.\n", C_STARTING1);
            strcpy(a.destination, C_STARTING2);
            makeAction(g, a);
            printf("UNI_C built a GO8 at '%s'.\n", C_STARTING2);
        }
        throwDice(g, 2);
        playersTested++;
    }

    printf("UNI_A should have 2 MJs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MJ));
    assert(getStudents(g, UNI_A, STUDENT_MJ) == 2);
    printf("UNI_A should have 0 MMONEYs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MMONEY));
    assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 0);
    printf("UNI_B should have 2 MJs. Returned %d.\n", getStudents(g, UNI_B, STUDENT_MJ));
    assert(getStudents(g, UNI_B, STUDENT_MJ) == 2);
    printf("UNI_B should have 0 MMONEYs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MMONEY));
    assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 0);
    printf("UNI_C should have 2 MJs. Returned %d.\n", getStudents(g, UNI_A, STUDENT_MJ));
    assert(getStudents(g, UNI_C, STUDENT_MJ) == 2);
    printf("UNI_C should have 0 MMONEYs. Returned %d.\n\n", getStudents(g, UNI_A, STUDENT_MMONEY));
    assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

    throwDice(g, 5);
    printf("Using throwDice to give each player one set of resources. Each player's MJs and MMONEYs should increment by 2 as each player owns a GO8 on such tiles.\n");

    currentStudent = getStudents(g, UNI_A, STUDENT_MMONEY);
    printf("UNI_A should now have 2 MMONEY. Returned %d.\n", currentStudent);
    assert(currentStudent == 2);

    currentStudent = getStudents(g, UNI_B, STUDENT_MMONEY);
    printf("UNI_B should now have 2 MMONEY. Returned %d.\n", currentStudent);
    assert(currentStudent == 2);

    currentStudent = getStudents(g, UNI_C, STUDENT_MMONEY);
    printf("UNI_C should now have 2 MMONEY. Returned %d.\n", currentStudent);
    assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 2);

    currentStudent = getStudents(g, UNI_A, STUDENT_MJ);
    printf("UNI_A should now have 4 MJ. Returned %d.\n", currentStudent);
    assert(currentStudent == 4);

    currentStudent = getStudents(g, UNI_B, STUDENT_MJ);
    printf("UNI_B should now have 4 MJ. Returned %d.\n", currentStudent);
    assert(currentStudent == 4);
    
    currentStudent = getStudents(g, UNI_C, STUDENT_MJ);
    printf("UNI_C should now have 4 MJ. Returned %d.\n", currentStudent);
    assert(currentStudent == 4);
    
    disposeGame(g);
    printf("Passed all throw dice tests\n\n");

}

void testExchangeRate(void) {
    printf("Testing getExchangeRate functions\n");    
    action a;
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame(disciplines, dice);
    int i;
    int j;
    int k;
    printf("Testing that initial exchange rates for all players for all students is 3\n");
    for(i=UNI_A; i <= UNI_C; i++){
        for(j = STUDENT_BPS; j <= STUDENT_MMONEY; j++){
            for(k = STUDENT_BPS; k <= STUDENT_MMONEY; k++){
                printf("Exchange rate from student %d to %d for player %d is %d\n", j, k, i, getExchangeRate(g, i, j, k));
                assert(getExchangeRate(g, i, j, k) == 3);
            }
        }
    }
    // Give players resources
    while (getStudents(g,UNI_C,STUDENT_MJ) < 300){
        throwDice(g,8);
    }
    while (getStudents(g,UNI_A,STUDENT_BPS) < 300){
        throwDice(g,3);
    }
    //Give the MJ,MTV and MMONEY retraining centre to UNI_A, BQN to UNI_B, BPS to UNI_C
    int playersTested = 0;
    a.actionCode = RETRAIN_STUDENTS;
    while(playersTested < 0){
        if (getWhoseTurn(g) == UNI_A){
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_BQN;
            while (getStudents(g, UNI_A, STUDENT_BQN) <= 7){
                makeAction(g, a);
            }
            a.disciplineTo = STUDENT_MJ;
            while (getStudents(g, UNI_A, STUDENT_MJ) <= 7){
                makeAction(g, a);
            }
            a.disciplineTo = STUDENT_MTV;
            while (getStudents(g, UNI_A, STUDENT_MTV) <= 7){
                makeAction(g, a);
            }

            a.actionCode = OBTAIN_ARC;
            strcpy(a.destination, MTV_RETRAINING);
            makeAction(g, a);

            strcpy(a.destination, "L");
            makeAction(g, a);
            strcpy(a.destination, MMONEY_RETRAINING);
            makeAction(g, a);
            strcpy(a.destination, MJ_RETRAINING);
            makeAction(g, a);

            a.actionCode = BUILD_CAMPUS;
            strcpy(a.destination, MTV_RETRAINING);
            makeAction(g, a);
            strcpy(a.destination, MMONEY_RETRAINING);
            makeAction(g, a);
            strcpy(a.destination, MJ_RETRAINING);
            makeAction(g, a);

        } else if (getWhoseTurn(g) == UNI_B){
            a.disciplineFrom = STUDENT_MTV;
            a.disciplineTo = STUDENT_BQN;
            while (getStudents(g, UNI_B, STUDENT_BQN) <= 6){
                makeAction(g, a);
            }
            a.disciplineTo = STUDENT_MJ;
            while (getStudents(g, UNI_B, STUDENT_MJ) <= 6){
                makeAction(g, a);
            }
            a.disciplineTo = STUDENT_BPS;
            while (getStudents(g, UNI_B, STUDENT_BPS) <= 6){
                makeAction(g, a);
            }

            a.actionCode = OBTAIN_ARC;
            strcpy(a.destination, BQN_RETRAINING);
            makeAction(g, a);

            a.actionCode = BUILD_CAMPUS;
            strcpy(a.destination, BQN_RETRAINING);
            makeAction(g, a);

        } else {
            a.disciplineFrom = STUDENT_MJ;
            a.disciplineTo = STUDENT_BQN;
            while (getStudents(g, UNI_C, STUDENT_BQN) <= 6){
                makeAction(g, a);
            }
            a.disciplineTo = STUDENT_BPS;
            while (getStudents(g, UNI_C, STUDENT_BPS) <= 6){
                makeAction(g, a);
            }
            a.disciplineTo = STUDENT_MTV;
            while (getStudents(g, UNI_C, STUDENT_MTV) <= 6){
                makeAction(g, a);
            }

            a.actionCode = OBTAIN_ARC;
            strcpy(a.destination, BPS_RETRAINING);
            makeAction(g, a);

            a.actionCode = BUILD_CAMPUS;
            strcpy(a.destination, BPS_RETRAINING);
            makeAction(g, a);

        }
        playersTested++;
    }

    printf("UNI_A now has a retraining centre for MJ, MMONEY and MTV\n"
           "UNI_B now has a retraining centre for BQN\n"
           "UNI_A now has a retraining centre for BPS\n");

    for (i = 0; i <= STUDENT_MMONEY; i++){
        printf("UNI_A needs 2 MJs to trade to student %d. Returned %d\n", i, getExchangeRate(g, UNI_A, STUDENT_MJ, i));
        assert(getExchangeRate(g, UNI_A, STUDENT_MJ, i) == 2);
    }

    for (i = 0; i <= STUDENT_MMONEY; i++){
        printf("UNI_A needs 2 MTVs to trade to student %d. Returned %d\n", i, getExchangeRate(g, UNI_A, STUDENT_MTV, i));
        assert(getExchangeRate(g, UNI_A, STUDENT_MTV, i) == 2);
    }

    for (i = 0; i <= STUDENT_MMONEY; i++){
        printf("UNI_A needs 2 MMONEYs to trade to student %d. Returned %d\n", i, getExchangeRate(g, UNI_A, STUDENT_MMONEY, i));
        assert(getExchangeRate(g, UNI_A, STUDENT_MMONEY, i) == 2);
    }

    for (i = 0; i <= STUDENT_MMONEY; i++){
        printf("UNI_B needs 2 BQNs to trade to student %d. Returned %d\n", i, getExchangeRate(g, UNI_B, STUDENT_BQN, i));
        assert(getExchangeRate(g, UNI_B, STUDENT_MJ, i) == 2);
    }

    for (i = 0; i <= STUDENT_MMONEY; i++){
        printf("UNI_C needs 2 BPs to trade to student %d. Returned %d\n", i, getExchangeRate(g, UNI_C, STUDENT_BPS, i));
        assert(getExchangeRate(g, UNI_C, STUDENT_MJ, i) == 2);
    }

    disposeGame(g);
    printf("Passed all exchange rate tests\n\n");
}

void testGetMost (void) {
    action a;
    printf ("Testing getMostARCs function\n");
    
    int disciplines[] = DEFAULT_DISCIPLINES;
    disciplines[0] = STUDENT_BQN;
    disciplines[2] = STUDENT_BQN;
    disciplines[7] = STUDENT_BQN;
    disciplines[11] = STUDENT_BPS;
    disciplines[16] = STUDENT_BPS;
    disciplines[18] = STUDENT_BPS;
    int dice[] = DEFAULT_DICE;
    dice[0] = 8;
    dice[7] = 8;
    dice[11] = 8;
    dice[16] = 8;
    Game g = newGame (disciplines, dice);
    throwDice(g, 8);
    throwDice(g, 8);
    while(getWhoseTurn(g) != UNI_A) {
        throwDice(g, 8);  
    }
    a.actionCode = OBTAIN_ARC;
    printf("getMostARCs should initially return NO_ONE. Returned %d.\n", getMostARCs(g));
    assert(getMostARCs(g) == 0);

    printf("UNI_A builds an ARC at '%s' and now has the most ARCs.\n", A_PATH1);
    strcpy(a.destination, A_PATH1);
    makeAction(g, a);
    assert(getMostARCs(g) == UNI_A);
    throwDice(g, 4);

    printf("UNI_B builds an ARC at '%s'. UNI_A should still have most ARCs.\n", B_PATH1);
    strcpy(a.destination, B_PATH1);
    makeAction(g, a);
    assert(getMostARCs(g) == UNI_A);
    throwDice(g, 4);
 
    printf("UNI_C builds an ARC at '%s'. UNI_A should still have most ARCs.\n", C_PATH1);
    strcpy(a.destination, C_PATH1);
    makeAction(g, a);
    assert(getMostARCs(g) == UNI_A);

    printf("UNI_C now builds an ARC at '%s' and now has the most ARCs.\n", C_PATH2);
    strcpy(a.destination, C_PATH2);
    makeAction(g, a);
    assert(getMostARCs(g) == UNI_C);

    printf("Passed all getMostARCs tests.\n\n Now testing getMostPublications.\n");

    disposeGame(g);
 
    disciplines[0] = STUDENT_MJ;
    disciplines[2] = STUDENT_MJ;
    disciplines[7] = STUDENT_MJ;
    disciplines[11] = STUDENT_MMONEY;
    disciplines[16] = STUDENT_MMONEY;
    disciplines[18] = STUDENT_MMONEY;

    dice[0] = 8;
    dice[7] = 8;
    dice[11] = 8;
    dice[16] = 8;
    g = newGame (disciplines, dice);   
    printf("getMostPublications should be initialised to NO_ONE. Returned %d.\n", getMostPublications(g));
    assert(getMostPublications(g) == NO_ONE);
    printf("Giving the players sufficiently high resources to perform the test properly\n");
    while (getTurnNumber(g) < 400){
        throwDice(g,8);
    }
    a.actionCode = RETRAIN_STUDENTS;
    a.disciplineFrom = STUDENT_MJ;
    a.disciplineTo = STUDENT_MTV;
    int playersTested = 0;
    while (playersTested < 3) {
        while (getStudents(g, getWhoseTurn(g), STUDENT_MTV) < 100){
            makeAction(g,a);
        }
        throwDice(g, 8);
        playersTested++;
    }
    
    while(getWhoseTurn(g) != UNI_A){
        throwDice(g, 8);
    }
    a.actionCode = OBTAIN_PUBLICATION;
    printf("UNI_A makes one publication and now has the most publications.\n");
    while (getPublications(g, UNI_A) != 1){
        makeAction(g, a);
    }
    assert(getMostPublications(g) == UNI_A);
    throwDice(g, 8);

    printf("UNI_B makes one publication, but UNI_A still has the most.\n");
    while (getPublications(g, UNI_B) != 1){
        makeAction(g, a);
    }
    assert(getMostPublications(g) == UNI_A);
    throwDice(g, 8);

    printf("UNI_C makes one publication, but UNI_A still has the most.\n");
    while (getPublications(g, UNI_C) != 1){
        makeAction(g, a);
    }
    assert(getMostPublications(g) == UNI_A);

    printf("UNI_C now makes another publication and now has the most.\n");
    while (getPublications(g, UNI_C) != 2){
        makeAction(g, a);
    }
    assert(getMostPublications(g) == UNI_C);

    printf("Passed all getMost related tests!\n\n");
}

void testIsLegalAction(void) {
    action a;
    printf ("Testing isLegalAction function\n");
    
    int disciplines[] = DEFAULT_DISCIPLINES;
    disciplines[0] = STUDENT_BQN;
    disciplines[2] = STUDENT_BQN;
    disciplines[7] = STUDENT_BQN;
    disciplines[11] = STUDENT_BPS;
    disciplines[16] = STUDENT_BPS;
    disciplines[18] = STUDENT_BPS;
    int dice[] = DEFAULT_DICE;
    dice[0] = 8;
    dice[7] = 8;
    dice[11] = 8;
    dice[16] = 8;
    Game g = newGame (disciplines, dice);
    throwDice(g, 7);
    printf("Testing actions without necessary resources.\n");

    int playersTested = 0;

    while (playersTested < 3){
        if (getWhoseTurn(g) == UNI_A){
            printf("Testing for UNI_A.\n");
            printf("Testing illegal building an ARC at '%s'.\n", A_PATH1);
            a.actionCode = OBTAIN_ARC;
            strcpy(a.destination, A_PATH1);
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal building a campus at '%s'.\n", A_PATH1);
            a.actionCode = BUILD_CAMPUS;
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal building an GO8 at '%s'.\n", A_STARTING1);
            a.actionCode = BUILD_CAMPUS;
            strcpy(a.destination, A_STARTING1);
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal obtaining a publication.\n");
            a.actionCode = OBTAIN_PUBLICATION;
            assert(isLegalAction(g, a) == FALSE);
           
            printf("Testing illegal obtaining an IP.\n");
            a.actionCode = OBTAIN_IP_PATENT;
            assert(isLegalAction(g, a) == FALSE);

        } else if (getWhoseTurn(g) == UNI_B) {
            printf("Testing for UNI_B.\n");
            printf("Testing illegal building an ARC at '%s'.\n", B_PATH1);
            a.actionCode = OBTAIN_ARC;
            strcpy(a.destination, B_PATH1);
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal building a campus at '%s'.\n", B_PATH1);
            a.actionCode = BUILD_CAMPUS;
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal building an GO8 at '%s'.\n", B_STARTING1);
            a.actionCode = BUILD_CAMPUS;
            strcpy(a.destination, B_STARTING1);
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal obtaining a publication.\n");
            a.actionCode = OBTAIN_PUBLICATION;
            assert(isLegalAction(g, a) == FALSE);
           
            printf("Testing illegal obtaining an IP.\n");
            a.actionCode = OBTAIN_IP_PATENT;
            assert(isLegalAction(g, a) == FALSE);

        } else {
            printf("Testing for UNI_C.\n");
            printf("Testing illegal building an ARC at '%s'.\n", C_PATH1);
            a.actionCode = OBTAIN_ARC;
            strcpy(a.destination, C_PATH1);
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal building a campus at '%s'.\n", C_PATH1);
            a.actionCode = BUILD_CAMPUS;
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal building an GO8 at '%s'.\n", C_STARTING1);
            a.actionCode = BUILD_CAMPUS;
            strcpy(a.destination, C_STARTING1);
            assert(isLegalAction(g, a) == FALSE);

            printf("Testing illegal obtaining a publication.\n");
            a.actionCode = OBTAIN_PUBLICATION;
            assert(isLegalAction(g, a) == FALSE);
           
            printf("Testing illegal obtaining an IP.\n");
            a.actionCode = OBTAIN_IP_PATENT;
            assert(isLegalAction(g, a) == FALSE);
        }
        throwDice(g, 7);
        playersTested++;
    }

    printf("Giving all three players plenty of resources.\n");
    int i = 0;
    while (i < 1000 || getWhoseTurn(g) != UNI_A){
        throwDice(g, 8);
        i++;
    }
    
    playersTested = 0;
    a.actionCode = RETRAIN_STUDENTS;
    while (playersTested < 3) {
        while(getStudents(g, getWhoseTurn(g), STUDENT_MJ) < 50){
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_MJ;
            makeAction(g, a);
        }
        while(getStudents(g, getWhoseTurn(g), STUDENT_MTV) < 50){
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_MTV;
            makeAction(g, a);
        }    
        while(getStudents(g, getWhoseTurn(g), STUDENT_MMONEY) < 50){
            a.disciplineFrom = STUDENT_BQN;
            a.disciplineTo = STUDENT_MMONEY;
            makeAction(g, a);
        }
        throwDice(g, 8);
        playersTested++;
    }

    while (getWhoseTurn(g) != UNI_A){
        throwDice(g, 8);
    } 

    printf("UNI_A builds an ARC at %s\n", A_PATH2);
    a.actionCode = OBTAIN_ARC;
    strcpy(a.destination, A_PATH2);
    makeAction(g, a);
    assert(getARC(g, A_PATH2) == UNI_A);
    
    printf("UNI_A builds an campus at %s\n", A_PATH2);
    a.actionCode = BUILD_CAMPUS;
    makeAction(g, a);
    assert(getCampus(g, A_PATH2) == UNI_A);
  
    printf("Testing illegal building of campuses at the three vertices around %s\n", A_PATH2);
    printf("Trying to illegally build a campus at %s\n", A_PATH5);
    strcpy(a.destination, A_PATH5);
    assert(isLegalAction == FALSE);

    printf("Trying to illegally build a campus at %s\n", A_PATH6);
    strcpy(a.destination, A_PATH6);
    assert(isLegalAction == FALSE);

    printf("Trying to illegally build a campus at %s\n", A_PATH7);
    strcpy(a.destination, A_PATH7);
    assert(isLegalAction == FALSE);

    printf("Passed tests involving vertices around a campus!\n\n");

    printf("Testing if the game is limited to 8 GO8s.\n");
    printf("UNI_A builds a GO8 at %s\n", A_PATH2);
    a.actionCode = BUILD_GO8;
    strcpy(a.destination, A_PATH2);
    makeAction(g, a);
    
    printf("UNI_A builds a GO8 at %s\n", A_STARTING1);
    strcpy(a.destination, A_STARTING1);
    makeAction(g, a);

    printf("UNI_A builds a GO8 at %s\n", A_STARTING2);
    strcpy(a.destination, A_STARTING2);
    makeAction(g, a);

    throwDice(g,8);

    printf("UNI_B builds a GO8 at %s\n", B_STARTING1);
    strcpy(a.destination, B_STARTING1);
    makeAction(g, a);

    printf("UNI_B builds a GO8 at %s\n", B_STARTING2);
    strcpy(a.destination, B_STARTING2);
    makeAction(g, a);

    a.actionCode = OBTAIN_ARC;
    strcpy(a.destination, B_PATH1);
    makeAction(g, a);

    a.actionCode = BUILD_CAMPUS;
    makeAction(g, a);

    printf("UNI_B builds a GO8 at %s\n", B_PATH1);
    a.actionCode = BUILD_GO8;
    makeAction(g, a);

    printf("UNI_C builds a GO8 at %s\n", C_STARTING1);
    strcpy(a.destination, C_STARTING1);
    makeAction(g, a);

    printf("UNI_C builds a GO8 at %s\n", C_STARTING2);
    strcpy(a.destination, C_STARTING2);
    makeAction(g, a);

    printf("There are should be now be 8 GO8s. Returned %d\n", getGO8s(g, UNI_A) + getGO8s(g, UNI_B) + getGO8s(g, UNI_C));
    assert(getGO8s(g, UNI_A) + getGO8s(g, UNI_B) + getGO8s(g, UNI_C) == 8);

    printf("UNI_C tries to illegaly build a GO8 at %s\n", C_PATH1);
    a.actionCode = OBTAIN_ARC;
    strcpy(a.destination, C_PATH1);
    makeAction(g, a);

    a.actionCode = BUILD_CAMPUS;
    makeAction(g, a);

    a.actionCode = BUILD_GO8;
    assert(isLegalAction(g, a) == FALSE);

    disposeGame(g);
    printf("Passed all isLegalAction functions!\n\n");
}
