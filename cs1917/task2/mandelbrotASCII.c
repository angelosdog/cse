//Program to print an ASCII version of the mandelbrot set
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define WIDTH 50
#define HEIGHT 50


void printMandelbrot(void);
int checkInMandelbrot(double x, double y);


int main(int argc, char* argv[]){

	printMandelbrot();

  return 0;
}
void printMandelbrot(void){
	int x, y;
	for(y=0;y<=HEIGHT;y++){
		for(x=0; x<=WIDTH ;x++){
			if (checkInMandelbrot(x, y) == 1){
				printf("#");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

int checkInMandelbrot(double xC, double yC){
	int iteration = 0;
	int maxIteration = 1000;
	int inMandelbrot;
	double xtemp, x, y;
	x = 0;
	y = 0;
	
// xC must be within [-2.5, 1]
// account for xC = 0 (you cant divide by 0)
/*	if (xC == 0){
		xC = -2.5;
	} else {
		xC =  - 2.5;
	}
*/

	xC = xC*7/100 - 2;
/*	if (yC == 0){
		yC = -1;
	} else {
	i	yC = 2/HEIGHT - 1;
	}i
*/
	yC = yC*1.5/25 - 1;

	
	while (x*x + y*y < 4 && iteration < maxIteration){
		xtemp = x*x - y*y + xC;
		y = 2*x*y + yC;
		x = xtemp;
		iteration++;
	}
	if (iteration < 1000){
		inMandelbrot = 0;
	} else {
		inMandelbrot = 1;
	}

	return inMandelbrot;
}
