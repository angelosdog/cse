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

color mandelbrotPixelColor(double xC, double yC, double centreX, double centreY, int zoom){
	int iterations = 0;
	double xtemp;
	double x = 0;
	double y = 0;
//xscaled = (xC) * 2^(-zoom) + xcenter
// to get a nice circle, we need xC and yC range for the bounds to be the same
// after some testing, -2 <= xC <= 1 and -1.5 <= yC <= 1.5 give the cleanest results
	xC = ldexp(xC, -zoom) + centreX;
	yC = ldexp(yC, -zoom) + centreY;
	while (x*x + y*y < 4 && iterations < MAX_ITERATIONS){
		xtemp = x*x - y*y + xC;
		y = 2*x*y + yC;
		x = xtemp;
		iterations++;
	}
	color pixel;
	pixel.red = stepsToRed(iterations, x, y);
	pixel.green = stepsToGreen(iterations, x, y);
	pixel.blue = stepsToBlue(iterations, x, y);
	return pixel;
}
