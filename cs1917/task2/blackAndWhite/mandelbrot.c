//Program to print an black and white version of the mandelbrot set
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pixelColor.h"
#include "mandelbrot.h"
#define WIDTH 512
#define HEIGHT 512
#define MAX_ITERATIONS 256

/*char insertMandelbrot(void){
	char RGBValue[512*512*3 - 53];
	int i = 0;
	int steps;
	int x, y, iterations;
	for(y=HEIGHT;y>=0;y++){
// Hex editor edits pixels right to left, top to bottom
		for(x=0; x<=WIDTH ;x++){
*
iterations = checkInMandelbrot(x, y);
Use some sort of insert fucntion which relies on
decValue.ToString(stepsToRed(iterations))
and so on for all three colours
*	//	iterations = checkInMandelbrot(x,y,*iterations);
			while (i < sizeof(RGBValue)){
				RGBValue[i] = stepsToRed(iterations);
				RGBValue[i+1] = stepsToGreen(iterations);
				RGBValue[i+2] = stepsToBlue(iterations);
				i += 3;
			}
		}	
	}
	return RGBValue[sizeof(RGBValue)];
}
*/
int checkInMandelbrot(double xC, double yC, double centreX, double centreY, int zoom){
	int iterations = 0;
	double xtemp;
	double x = 0;
	double y = 0;
// to get a nice circle, we need xC and yC range for the bounds to be the same
// after some testing, -2 <= xC <= 1 and -1.5 <= yC <= 1.5 give the cleanest results
	xC = ldexp((xC*3/512 - 2)+centreX,-zoom);
	yC = ldexp((yC*3/512 - 1.5)+centreY,-zoom);
	
	while (x*x + y*y < ldexp(4,-zoom) && iterations < MAX_ITERATIONS){
		xtemp = x*x - y*y + xC;
		y = 2*x*y + yC;
		x = xtemp;
		iterations++;
	}
	
	return iterations;
}
