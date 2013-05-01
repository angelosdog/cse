/*
 *  ai.c v2.0
 *  This ai will rush to build a campus at a retraining centre, build two more campuses, convert 2 campuses to GO8 amd then spinoff 
 *
 *  Created by Richard Buckland on 14/05/11.
 *  Copyright 2011 Licensed under Creative Commons SA-BY-NC 3.0. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Game.h"
#include "ai.h"

#define A_PATH1 "RR"
#define A_PATH2 "RLRLRLRLRLLLL"
#define A_PATH3 "RLRLRLRLRLLLLR"
#define A_PATH4 "RLRLRLRLRLLLLRL"
#define A_PATH6 "RRL"
#define A_PATH7 "RLRLRLRLRLLL"
#define A_PATH8 "RLRLRLRLRLLLLR"
#define A_PATH9 "RLRLRLRLRLLLLL"
#define A_PATH10 "RLRLRLRLRLLLLR"
#define A_PATH11 "RLRLRLRLRLLLLRLR"
#define A_PATH12 "RLRLRLRLRLLLLRLL"

#define A_STARTING1 "RB"
#define A_STARTING2 "RLRLRLRLRLLB"

#define B_PATH1 "LRLRLRRLRLBR"
#define B_PATH2 "RRLRLLL"
#define B_PATH3 "RRLRLLLR"
#define B_PATH4 "RRLRLLLRL"
#define B_PATH6 "LRLRLRRLRLBRL"
#define B_PATH7 "RRLRLLLL"
#define B_PATH8 "RRLRLL"
#define B_PATH9 "RRLRLLLR"
#define B_PATH10 "RRLRLLLR"
#define B_PATH11 "RRLRLLLRLR"
#define B_PATH12 "RRLRLLLRLL"

#define B_STARTING1 "RRLRLLB"
#define B_STARTING2 "LRLRLRRLRL"

#define C_PATH1 "LRLRLBR"
#define C_PATH2 "LRLRLBL"
#define C_PATH3 "LRLRLBLR"
#define C_PATH4 "LRLRLBLRL"
#define C_PATH6 "LRLRLBRL"
#define C_PATH7 "LRLRLBLR"
#define C_PATH8 "LRLRLB"
#define C_PATH9 "LRLRLBLL"
#define C_PATH10 "LRLRLBLRLR"
#define C_PATH11 "LRLRLBLRLL"
#define C_PATH12 "LRLRLBLR"

#define C_STARTING1 "LRLRLB"
#define C_STARTING2 "RRLRLLRLRLLB"


#define NO_PATH "\0"
action tradeForARCs(Game g, action nextAction, int BPS, int BQN, int player);
action generatePath(Game g, action nextAction);
action makeSpinoff(Game g, action nextAction, int player);
action tradeForCampuses(Game g, action nextAction, int MJ, int MTV, int BPS, int BQN, int player);
action buildGO8(Game g, action nextAction, int player, int BPS, int BQN, int MJ, int MTV, int MMONEY);
action generatePathGO8(Game g, action nextAction);

action decideAction (Game g) {
    int player = getWhoseTurn(g);
    int BPS = getStudents(g, player, STUDENT_BPS);  
    int BQN = getStudents(g, player, STUDENT_BQN);  
    int MJ = getStudents(g, player, STUDENT_MJ);  
    int MTV = getStudents(g, player, STUDENT_MTV);
    int MMONEY = getStudents(g, player, STUDENT_MMONEY);
    int pathBlocked = FALSE;
    int campuses = getCampuses(g, player);
    int GO8s = getGO8s(g, player);
    action nextAction;
    nextAction.disciplineFrom = 0;
    nextAction.disciplineTo = 0;
    int i;
    for (i = 0; i < PATH_LIMIT; i++){
        nextAction.destination[i] = '\0';
    }
    nextAction.actionCode = PASS;

    // Check if preset path has been blocked


    if (player == UNI_A) {
        if (getARC(g, A_PATH1) != player && getARC(g, A_PATH1) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, A_PATH2) != player && getARC(g, A_PATH2) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, A_PATH3) != player && getARC(g, A_PATH3) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, A_PATH4) != player && getARC(g, A_PATH4) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, A_PATH1) != player && getCampus(g, A_PATH1) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, A_PATH2) != player && getCampus(g, A_PATH2) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, A_PATH4) != player && getCampus(g, A_PATH4) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, A_PATH6) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", A_PATH6);
        } else if (getCampus(g, A_PATH7) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", A_PATH7);
        } else if (getCampus(g, A_PATH8) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", A_PATH8);
        } else if (getCampus(g, A_PATH9) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", A_PATH9);
        } else if (getCampus(g, A_PATH10) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", A_PATH10);
        } else if (getCampus(g, A_PATH11) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", A_PATH11);
        } else if (getCampus(g, A_PATH12) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", A_PATH12);
        }

    } else if (player == UNI_B) {
        if (getARC(g, B_PATH1) != player && getARC(g, B_PATH1) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, B_PATH2) != player && getARC(g, B_PATH2) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, B_PATH3) != player && getARC(g, B_PATH3) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, B_PATH4) != player && getARC(g, B_PATH4) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, B_PATH1) != player && getCampus(g, B_PATH1) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, B_PATH2) != player && getCampus(g, B_PATH2) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, B_PATH4) != player && getCampus(g, B_PATH4) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, B_PATH6) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", B_PATH6);
        } else if (getCampus(g, B_PATH7) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", B_PATH7);
        } else if (getCampus(g, B_PATH8) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", B_PATH8);
        } else if (getCampus(g, B_PATH9) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", B_PATH9);
        } else if (getCampus(g, B_PATH10) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", B_PATH10);
        } else if (getCampus(g, B_PATH11) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", B_PATH11);
        } else if (getCampus(g, B_PATH12) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", B_PATH12);
        }

    } else {
        if (getARC(g, C_PATH1) != player && getARC(g, C_PATH1) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, C_PATH2) != player && getARC(g, C_PATH2) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, C_PATH3) != player && getARC(g, C_PATH3) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getARC(g, C_PATH4) != player && getARC(g, C_PATH4) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, C_PATH1) != player && getCampus(g, C_PATH1) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, C_PATH2) != player && getCampus(g, C_PATH2) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, C_PATH4) != player && getCampus(g, C_PATH4) != NO_ONE) {
            pathBlocked = TRUE;
        } else if (getCampus(g, C_PATH6) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", C_PATH6);
       } else if (getCampus(g, C_PATH7) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", C_PATH7);
       } else if (getCampus(g, C_PATH8) != NO_ONE) {
            pathBlocked = TRUE;
 printf("%s is blocked\n", C_PATH8);
       } else if (getCampus(g, C_PATH9) != NO_ONE) {
            pathBlocked = TRUE;
printf("%s is blocked\n", C_PATH9);

        } else if (getCampus(g, C_PATH10) != NO_ONE) {
            pathBlocked = TRUE;
printf("%s is blocked\n", C_PATH10);
        } else if (getCampus(g, C_PATH11) != NO_ONE) {
            pathBlocked = TRUE;
printf("%s is blocked\n", C_PATH11);
        } else if (getCampus(g, C_PATH12) != NO_ONE) {
            pathBlocked = TRUE;
printf("%s is blocked\n", C_PATH12);

        }

    }
    
    if (campuses < 5 && GO8s == 0) {
        if (pathBlocked == FALSE) {
            if (BQN == 0 || BPS == 0){
                nextAction = tradeForARCs(g, nextAction, BPS, BQN, player);
            } else {
                if (player == UNI_A) { 
                    if (getARC(g, A_PATH1) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, A_PATH1) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    } else if (getARC(g, A_PATH2) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, A_PATH2) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    } else if (getARC(g, A_PATH3) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getARC(g, A_PATH4) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                     } else if (getCampus(g, A_PATH4) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    }
                } else if (player == UNI_B) {
                     if (getARC(g, B_PATH1) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, B_PATH1) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    } else if (getARC(g, B_PATH2) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, B_PATH2) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    } else if (getARC(g, B_PATH3) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getARC(g, B_PATH4) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, B_PATH4) != player) {
                         if (MTV == 0 || MJ == 0){
                             nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                         } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    } 
                } else {
                    if (getARC(g, C_PATH1) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, C_PATH1) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    } else if (getARC(g, C_PATH2) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, C_PATH2) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }  
                    } else if (getARC(g, C_PATH3) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getARC(g, C_PATH4) != player){ 
                        nextAction.actionCode = OBTAIN_ARC;
                        nextAction = generatePath(g, nextAction);
                    } else if (getCampus(g, C_PATH4) != player) {
                        if (MTV == 0 || MJ == 0){
                            nextAction = tradeForCampuses(g, nextAction, MJ, MTV, BPS, BQN, player);
                        } else {
                            nextAction.actionCode = BUILD_CAMPUS;
                            nextAction = generatePath(g, nextAction);
                        }
                    }
                }
            } 
        } else {
            printf("Path blocked\n");
            if (GO8s < 2){
                nextAction = buildGO8(g, nextAction, player, BPS, BQN, MJ, MTV, MMONEY);
            } else {
                nextAction = makeSpinoff(g, nextAction, player);
            } 
        }
    } else if (GO8s < 2){ // 2 is the magic number!
            nextAction = buildGO8(g, nextAction, player, BPS, BQN, MJ, MTV, MMONEY);
    } else {
            nextAction = makeSpinoff(g, nextAction, player);
    }

    return nextAction;
}

action buildGO8(Game g, action nextAction, int player, int BPS, int BQN, int MJ, int MTV, int MMONEY){
    int exchangeBPS = getExchangeRate(g, player, STUDENT_BPS, STUDENT_MMONEY);
    int exchangeBQN = getExchangeRate(g, player, STUDENT_BQN, STUDENT_MMONEY);
    int exchangeMJ = getExchangeRate(g, player, STUDENT_MJ, STUDENT_MMONEY);
    int exchangeMTV = getExchangeRate(g, player, STUDENT_MTV, STUDENT_MMONEY);
    int exchangeMMONEY = getExchangeRate(g, player, STUDENT_MMONEY, STUDENT_MTV);


    // At this point, we do not really care that much for MTVs. Just make sure they don't get lost
    // when a 7 gets rolled.
    if (MMONEY < 3 || MJ < 2){
        if (MTV >= exchangeMTV){
            nextAction.disciplineTo = STUDENT_MJ;
            nextAction.disciplineFrom = STUDENT_MTV;
            nextAction.actionCode = RETRAIN_STUDENTS;

        } else if (MJ < 2){
            nextAction.disciplineTo = STUDENT_MJ;
            if (MMONEY > exchangeMMONEY){
                nextAction.disciplineFrom = STUDENT_MMONEY;
                nextAction.actionCode = RETRAIN_STUDENTS;  
            } else if (BPS >= exchangeBPS){
                nextAction.disciplineFrom = STUDENT_BPS;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (BQN >= exchangeBQN){
                nextAction.disciplineFrom = STUDENT_BQN;
                nextAction.actionCode = RETRAIN_STUDENTS;
            }

        } else if (MMONEY < 3){
            nextAction.disciplineTo = STUDENT_MMONEY;
            if (MMONEY  == 0) {
                if (MJ > 3*exchangeMJ + 1){
                    nextAction.disciplineFrom = STUDENT_MJ;
                    nextAction.actionCode = RETRAIN_STUDENTS;  
                } else if (BPS >= 3*exchangeBPS){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= 3*exchangeBQN){
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (MJ > 2*exchangeMJ + 1 && BPS >= exchangeBPS){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (MJ > 2*exchangeMJ + 1 && BQN >= exchangeBQN){
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BPS >= 2*exchangeBPS && BQN >= exchangeBQN){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BPS >= 2*exchangeBPS && MJ > exchangeMJ + 1 ){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= 2*exchangeBQN && BPS >= exchangeBPS){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= 2*exchangeBQN && MJ > exchangeMJ + 1){
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= exchangeBQN && BPS >= exchangeBPS && MJ > exchangeMJ + 1){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                }

            } else if (MMONEY == 1) {
                if (MJ > 2*exchangeMJ + 1){
                    nextAction.disciplineFrom = STUDENT_MJ;
                    nextAction.actionCode = RETRAIN_STUDENTS;  
                } else if (BPS >= 2*exchangeBPS){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= 2*exchangeBQN){
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= exchangeBQN && BPS >= exchangeBPS){
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= exchangeBQN && MJ > exchangeMJ + 1){
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BPS >= exchangeBPS && MJ > exchangeMJ + 1){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } 
           
            } else {
                if (MJ > exchangeMJ + 1){
                    nextAction.disciplineFrom = STUDENT_MJ;
                    nextAction.actionCode = RETRAIN_STUDENTS;  
                } else if (BPS >= exchangeBPS){
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                } else if (BQN >= exchangeBQN){
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.actionCode = RETRAIN_STUDENTS;
                }
            }
        }
    } else {
            nextAction.actionCode = BUILD_GO8;
            nextAction = generatePathGO8(g, nextAction);
    }
    return nextAction;
}

action generatePathGO8(Game g, action nextAction){
    if (getWhoseTurn(g) == UNI_A) {
        strcpy(nextAction.destination, A_PATH4);
        if (!(isLegalAction(g, nextAction))) {
            strcpy(nextAction.destination, A_PATH2);
            if(!(isLegalAction(g,nextAction))) {
                strcpy(nextAction.destination,A_PATH1);
                if(!(isLegalAction(g,nextAction))) {
                    strcpy(nextAction.destination,A_STARTING1);
                    if(!(isLegalAction(g,nextAction))) {
                        strcpy(nextAction.destination,A_STARTING2);
                        if(!(isLegalAction(g,nextAction))) {
                            nextAction.actionCode = PASS;
                        }
                    }
                }
            }
        }    
    } else if (getWhoseTurn(g) == UNI_B) {
        strcpy(nextAction.destination, B_PATH4);
        if (!(isLegalAction(g, nextAction))) {
            strcpy(nextAction.destination, B_PATH2);
            if(!(isLegalAction(g,nextAction))) {
                strcpy(nextAction.destination,B_PATH1);
                if(!(isLegalAction(g,nextAction))) {
                    strcpy(nextAction.destination,B_STARTING1);
                    if(!(isLegalAction(g,nextAction))) {
                        strcpy(nextAction.destination,B_STARTING2);
                        if(!(isLegalAction(g,nextAction))) {
                            nextAction.actionCode = PASS;
                        }
                    }
                }
            }
        }
    } else {
        strcpy(nextAction.destination, C_PATH4);
        if (!(isLegalAction(g, nextAction))) {
            strcpy(nextAction.destination, C_PATH2);
            if(!(isLegalAction(g,nextAction))) {
                strcpy(nextAction.destination,C_PATH1);
                if(!(isLegalAction(g,nextAction))) {
                    strcpy(nextAction.destination,C_STARTING1);
                    if(!(isLegalAction(g,nextAction))) {
                        strcpy(nextAction.destination,C_STARTING2);
                        if(!(isLegalAction(g,nextAction))) {
                            nextAction.actionCode = PASS;
                        }
                    }
                }
            }
        }
     }
    return nextAction;
}


action tradeForARCs(Game g, action nextAction, int BPS, int BQN, int player){
    int studentToExchange = STUDENT_MMONEY;
    if (BPS == 0){
        nextAction.disciplineTo = STUDENT_BPS;
        if (BQN > getExchangeRate(g, player, nextAction.disciplineTo, STUDENT_BPS)){
            nextAction.disciplineFrom = STUDENT_BQN;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else {
            while(studentToExchange > STUDENT_BQN && nextAction.actionCode != RETRAIN_STUDENTS){
                nextAction.disciplineFrom = studentToExchange;
                if (getStudents(g, player, studentToExchange) >= getExchangeRate(g, player, studentToExchange, nextAction.disciplineTo)){
                    nextAction.actionCode = RETRAIN_STUDENTS;
                }
                studentToExchange--;
            }
        }
    } else if (BQN == 0){
        nextAction.disciplineTo = STUDENT_BQN;
        if (BPS > getExchangeRate(g, player, nextAction.disciplineTo, STUDENT_BPS)){
            nextAction.disciplineFrom = STUDENT_BPS;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else {
            while(studentToExchange > STUDENT_BQN && nextAction.actionCode != RETRAIN_STUDENTS) {
                nextAction.disciplineFrom = studentToExchange;
                if (getStudents(g, player, studentToExchange) >= getExchangeRate(g, player, studentToExchange, nextAction.disciplineTo)){
                    nextAction.actionCode = RETRAIN_STUDENTS;      
                }
                studentToExchange--;
            }    
        }
    }

    return nextAction;
}

action tradeForCampuses(Game g, action nextAction, int MJ, int MTV, int BPS, int BQN, int player){
    if (MJ == 0){
        nextAction.disciplineTo = STUDENT_MJ;
        if (getStudents(g, player, STUDENT_MMONEY) >= (getExchangeRate(g, player, STUDENT_MMONEY, nextAction.disciplineTo ))){
            nextAction.disciplineFrom = STUDENT_MMONEY;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else if (MTV > getExchangeRate(g, player, STUDENT_MTV, nextAction.disciplineTo)){
            nextAction.disciplineFrom = STUDENT_MTV;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else if (BPS > getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo)){
            nextAction.disciplineFrom = STUDENT_BPS;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else if (BQN > getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo)){
            nextAction.disciplineFrom = STUDENT_BQN;
            nextAction.actionCode = RETRAIN_STUDENTS;
       } 
    } else if (MTV == 0){
        nextAction.disciplineTo = STUDENT_MTV;
        if (getStudents(g, player, STUDENT_MMONEY) >= (getExchangeRate(g, player, STUDENT_MMONEY, nextAction.disciplineTo))){
            nextAction.disciplineFrom = STUDENT_MMONEY;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else if (MJ > getExchangeRate(g, player, STUDENT_MJ, nextAction.disciplineTo)){
            nextAction.disciplineFrom = STUDENT_MJ;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else if (BPS > getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo)){
            nextAction.disciplineFrom = STUDENT_BPS;
            nextAction.actionCode = RETRAIN_STUDENTS;
        } else if (BQN > getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo)){
            nextAction.disciplineFrom = STUDENT_BQN;
            nextAction.actionCode = RETRAIN_STUDENTS;
        }
    }
    return nextAction;
}
action generatePath(Game g, action nextAction) {
    if (getWhoseTurn(g) == UNI_A) {
        strcpy(nextAction.destination, A_PATH1);
        if (!(isLegalAction(g, nextAction))) {
            strcpy(nextAction.destination, A_PATH2);
            if(!(isLegalAction(g,nextAction))) {
                strcpy(nextAction.destination,A_PATH3);
                if(!(isLegalAction(g,nextAction))) {
                    strcpy(nextAction.destination,A_PATH4);
                    if(!(isLegalAction(g,nextAction))) {
                        nextAction.actionCode = PASS;
                    }
                }
            }
        }    
    } else if (getWhoseTurn(g) == UNI_B) {
        strcpy(nextAction.destination, B_PATH1);
        if (!(isLegalAction(g, nextAction))) {
            strcpy(nextAction.destination, B_PATH2);
            if(!(isLegalAction(g,nextAction))) {
                strcpy(nextAction.destination,B_PATH3);
                if(!(isLegalAction(g,nextAction))) {
                    strcpy(nextAction.destination,B_PATH4);
                    if(!(isLegalAction(g,nextAction))) {
                        nextAction.actionCode = PASS;
                    }
                }
            }
        }
    } else {
        strcpy(nextAction.destination, C_PATH1);
        if (!(isLegalAction(g, nextAction))) {
            strcpy(nextAction.destination, C_PATH2);
            if(!(isLegalAction(g,nextAction))) {
                strcpy(nextAction.destination,C_PATH3);
                if(!(isLegalAction(g,nextAction))) {
                    strcpy(nextAction.destination,C_PATH4);
                    if(!(isLegalAction(g,nextAction))) {
                        nextAction.actionCode = PASS;
                    }
                }
            }
        }
     }
    return nextAction;
}

action makeSpinoff(Game g, action nextAction, int player){
    int BPS = getStudents(g, player, STUDENT_BPS);  
    int BQN = getStudents(g, player, STUDENT_BQN);  
    int MJ = getStudents(g, player, STUDENT_MJ);  
    int MTV = getStudents(g, player, STUDENT_MTV);  
    int MMONEY = getStudents(g, player, STUDENT_MMONEY);  
    if (MMONEY == 0 || MTV == 0 || MJ == 0){
        if (MJ == 0){
            nextAction.disciplineTo = STUDENT_MJ;
            if (MTV > getExchangeRate(g, player, STUDENT_MTV, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_MTV;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (MMONEY > getExchangeRate(g, player, STUDENT_MMONEY, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_MMONEY;
                nextAction.actionCode = RETRAIN_STUDENTS;  
            } else if (BPS >= getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BPS;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (BQN >= getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BQN;
                nextAction.actionCode = RETRAIN_STUDENTS;
            }

        } else if (MTV == 0 && MMONEY == 0){
             nextAction.disciplineTo = STUDENT_MTV;
             if (MJ > 2*(getExchangeRate(g, player, STUDENT_MJ, nextAction.disciplineTo))){
                nextAction.disciplineFrom = STUDENT_MJ;
                nextAction.actionCode = RETRAIN_STUDENTS;  
            } else if (BPS >= 2*(getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo))){
                nextAction.disciplineFrom = STUDENT_BPS;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (BQN >= 2*(getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo))){
                nextAction.disciplineFrom = STUDENT_BQN;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (BQN >= getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo) && BPS >= getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BQN;
                nextAction.actionCode = RETRAIN_STUDENTS;
            }  else if (BQN >= getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo) && MJ > getExchangeRate(g, player, STUDENT_MJ, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BQN;
                nextAction.actionCode = RETRAIN_STUDENTS;
            }  else if (BPS >= getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo) && MJ > getExchangeRate(g, player, STUDENT_MJ, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BPS;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } 

           
        } else if (MTV == 0){
            nextAction.disciplineTo = STUDENT_MTV;
            if (MMONEY > getExchangeRate(g, player, STUDENT_MMONEY, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_MMONEY;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (MJ > getExchangeRate(g, player, STUDENT_MJ, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_MJ;
                nextAction.actionCode = RETRAIN_STUDENTS;  
            } else if (BPS >= getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BPS;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (BQN >= getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BQN;
                nextAction.actionCode = RETRAIN_STUDENTS;
            }

        } else {
            nextAction.disciplineTo = STUDENT_MMONEY;
            if (MTV > getExchangeRate(g, player, STUDENT_MTV, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_MTV;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (MJ > getExchangeRate(g, player, STUDENT_MJ, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_MJ;
                nextAction.actionCode = RETRAIN_STUDENTS;  
            } else if (BPS >= getExchangeRate(g, player, STUDENT_BPS, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BPS;
                nextAction.actionCode = RETRAIN_STUDENTS;
            } else if (BQN >= getExchangeRate(g, player, STUDENT_BQN, nextAction.disciplineTo)){
                nextAction.disciplineFrom = STUDENT_BQN;
                nextAction.actionCode = RETRAIN_STUDENTS;
            }
        }
    } else {
        nextAction.actionCode = START_SPINOFF;
    }
    return nextAction;
}

