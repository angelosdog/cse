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
#include "testGame.h"
#define WIN_SCORE 150

/* 
 * These structs definitely need a new home, most likely Game.c when that's written.
 * Also I think we could cut down the amount of structs and use more arrays.
 * (these are TODO notes btw)
 */

// This funtion is to print out the game board
void printBoard(Game g);

// This function used to determine what number to print in the middle of the hexagon
int diceValueOfRegion(Game g, int region);

// This function is used to determine what colour to paint each individual hexagon based off the discipline associated with that region
char* disciplineOfRegion(Game g, int region);

int main(int argc, char* argv[]){

    Game g;
    int disciplines[NUM_REGIONS]; //NUM_REGIONS #defined in game.h
    int dice[NUM_REGIONS];
    g = newGame(disciplines, dice);

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
        // The loop next loop braks when the a.actionCode == 0, so we need to reset it
        a.actionCode = -1;

        /* rand() will return a pseduo random number between 0 and RAND_MAX
         * use %6 to get something between 0 to 5
         */

        diceScore = rand()%6 + rand()%6 + 2; // [FIXED] I'm not sure what the random function is
        	
        throwDice(g, diceScore);

        player = getWhoseTurn(g); //turnNumber%3+1
        while (a.actionCode != PASS){	
            // reset the values of legalAction and a.actionCode so that the loop at the end will repeat
            // until the actionCode is actually PASS
	    legalAction = FALSE;
            a.actionCode = -1;

            printBoard(g);
                       // I still haven't figured out how to print out the board yet, but I'm making it so that it prints out the
            // resources for everyone. Separate by tabs until I figure out how to make it look nicer
            printf("UNI_A            \t UNI_B           \t UNI_C\n"
                   "KPI: %d          \t KPI: %d         \t KPI: %d\n"
                   "Campuses: %d     \t Campuses: %d    \t Campuses: %d\n"
                   "GO8s: %d         \t GO8s: %d        \t GO8s: %d\n"
                   "ARC Grants: %d   \t ARC Grants: %d  \t ARC Grants: %d\n"
                   "IPs: %d          \t IPs: %d         \t IPs: %d\n"
                   "Publications: %d \t Publications: %d\t Publications: %d\n"
                   "THDs: %d         \t THDs: %d        \t THDs: %d\n"
                   "BPs: %d          \t BPs: %d         \t BPs: %d\n"
                   "BQNs: %d         \t BQNs: %d        \t BQNs: %d\n"
                   "MJs: %d          \t MJs: %d         \t MJs: %d\n"
                   "MTVs: %d         \t MTVs: %d        \t MTVs: %d\n"
                   "MMONEY: %d       \t MMONEY: %d      \t MMONEY: %d\t\n",
                   getKPIpoints(g, UNI_A), getKPIpoints(g, UNI_B), getKPIpoints(g, UNI_C),
                   getCampuses(g, UNI_A), getCampuses(g, UNI_B), getCampuses(g, UNI_C),
                   getGO8s(g, UNI_A), getGO8s(g, UNI_B), getGO8s(g, UNI_C),
                   getARCs(g, UNI_A), getARCs(g, UNI_B), getARCs(g, UNI_C), 
                   getIPs(g, UNI_A), getIPs(g, UNI_B), getIPs(g, UNI_C),
                   getPublications(g, UNI_A), getPublications(g, UNI_B), getPublications(g, UNI_C),

                   getStudents(g, UNI_A, STUDENT_THD), getStudents(g, UNI_B, STUDENT_THD), getStudents(g, UNI_C, STUDENT_THD),
                   getStudents(g, UNI_A, STUDENT_BPS), getStudents(g, UNI_B, STUDENT_BPS), getStudents(g, UNI_C, STUDENT_BPS),
                   getStudents(g, UNI_A, STUDENT_BQN), getStudents(g, UNI_B, STUDENT_BQN), getStudents(g, UNI_C, STUDENT_BQN),
                   getStudents(g, UNI_A, STUDENT_MJ), getStudents(g, UNI_B, STUDENT_MJ), getStudents(g, UNI_C, STUDENT_MJ),
                   getStudents(g, UNI_A, STUDENT_MTV), getStudents(g, UNI_B, STUDENT_MTV), getStudents(g, UNI_C, STUDENT_MTV),
                   getStudents(g, UNI_A, STUDENT_MMONEY), getStudents(g, UNI_B, STUDENT_MMONEY), getStudents(g, UNI_C, STUDENT_MMONEY));

            printf("Enter an action code\n"
                   "Pass: 0\n"
                   "Build Campus: 1\n"
                   "Build GO8: 2\n"
                   "Obtain ARC Grant: 3\n"
                   "Start Spinoff: 4\n"
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

void printBoard(Game g){
    /* print out a board that looks like this:
    
    Line 1:                      ____               
    Line 2:                     /    \
    Line 3:                ____/  11  \____         
    Line 4:               /    \      /    \        
    Line 5:          ____/  3   \____/  5   \____   
    Line 6:         /    \      /    \      /    \  
    Line 7:        /  6   \____/   1  \____/  9   \ 
    Line 8:        \      /    \      /    \      / 
    Line 9:         \____/  4   \____/  2   \____/  
    Line 10:        /    \      /    \      /    \  
    Line 11:       /  10  \____/  7   \____/  4   \ 
    Line 12:       \      /    \      /    \      / 
    Line 13:        \____/  9   \____/  5   \____/  
    Line 14:        /    \      /    \      /    \ 
    Line 15:       /  8   \____/  12  \____/  8   \ 
    Line 16:       \      /    \      /    \      / 
    Line 17:        \____/  6   \____/  10  \____/  
    Line 18:             \      /    \      /       
    Line 19:              \____/  3   \____/        
    Line 20:                   \      /             
    Line 21:                    \____/              

    To do this, we print each line of the map individually, and
    call the functions diceValueOfRegion to change the number in
    the centre of the hexagon, and disciplineOfRegion to colour
    each hexagon.
    */

}

int diceValueOfRegion (Game g, int region){


}

char* disciplineOfRegion (Game g, int region){


}
