#include "pixelColor.h"
#include <math.h>
#define MAX_ITERATIONS 256

unsigned char stepsToRed (int steps){
	unsigned char redValue;
		redValue = steps/100;
	return redValue;
}

unsigned char stepsToGreen (int steps){
	unsigned char greenValue;
		greenValue = steps/50;	
	return greenValue;
}

unsigned char stepsToBlue (int steps){
	unsigned char blueValue;
		blueValue = steps;	
	return blueValue;
}

