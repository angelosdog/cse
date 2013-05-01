// Program to print flags
// Tim Lambert 7 Mar 2012

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"

#define WIDTH 60
#define HEIGHT 30

void printOldLibyanFlag(void);
void printIrishFlag(void);
void printIndonesianFlag(void);
void printJapaneseFlag(void);
int checkInCircle(double x, int y);

int main(int argc, char* argv[]){

	printJapaneseFlag();
//  printOldLibyanFlag();

//  printIrishFlag();

//  printIndonesianFlag();
  
//  printf(WHITE);

  return 0;
}

void printOldLibyanFlag(void){
  int i,j;
  printf(GREEN);
  for(i=0; i < HEIGHT; i++) {
    for(j=0; j < WIDTH; j++) {
      printf("G");
    }
    printf("\n");
  }

}

void printIndonesianFlag(void){
  int i,j;
  printf(RED);
  for(i=0; i < HEIGHT/2; i++) {
    for(j=0; j < WIDTH; j++) {
      printf("R");
    }
    printf("\n");
  }
  printf(WHITE);
  for(i=0; i < HEIGHT/2; i++) {
    for(j=0; j < WIDTH; j++) {
      printf("w");
    }
    printf("\n");
  }

}

void printIrishFlag(void){
  int i,j;
  for(i=0; i < 8; i++) {
    printf(GREEN);
    for(j=0; j < 8; j++) {
      printf("G");
    }
    printf(WHITE);
    for(j=9; j < 16; j++) {
      printf("W");
    }
    printf(YELLOW);
    for(j=0; j < 8; j++) {
      printf("O");
    }
    printf("\n");
  }

}

void printJapaneseFlag(void){
	int i, j;
	for(i=0;i<HEIGHT;i++){
		for(j=0; j<WIDTH ;j++){
			if (checkInCircle(j, i)){
				printf(RED);
				printf("R");
			} else {
				printf(WHITE);
				printf("W");
			}
		}
		printf("\n");
	}
}

int checkInCircle(double x, int y){
	double radius = WIDTH/6;
	double centreX = WIDTH/2;
	double centreY = HEIGHT/2;
	int inCircle;
	if ((x - centreX)*(x - centreX) + (y - centreY)*(y - centreY) <= radius*radius){
		inCircle = 1;
	} else {
		inCircle = 0;
	}
	return inCircle;
}
