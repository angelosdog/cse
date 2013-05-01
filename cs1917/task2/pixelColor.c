/*
 * Calculates the colour of individual pixels of the
 * mandelbrot set based on the number of iterations
 * taken for a point to either leave the mandelbrot
 * set or enough iterations that it is certainly part
 * of the set
 * 
 * Created by Vincent Tran and Tony San
 */

#include "pixelColor.h"
#include <math.h>

#define ORIGIN 0
#define FIRST_OCTANT 32
#define SECOND_OCTANT 64
#define THIRD_OCTANT 96
#define FOURTH_OCTANT 128
#define FIFTH_OCTANT 160
#define SIXTH_OCTANT 192
#define SEVENTH_OCTANT 224
#define REVOLUTION 256

// divide the maximum number of iterations (256) into 8 portions
// for each octant, we want a certain colour
// some octants will have the same colour as the previous
// octant however

unsigned char stepsToRed (int steps){
	unsigned char redValue;
	if (steps >= ORIGIN && steps < FIRST_OCTANT){
		redValue = 0;
	} else if (steps >= FIRST_OCTANT && steps < SECOND_OCTANT){
		redValue = steps/5;
	} else if (steps >= SECOND_OCTANT && steps < THIRD_OCTANT){
		redValue = steps/4;
	} else if (steps >= THIRD_OCTANT && steps < FOURTH_OCTANT){
		redValue = steps/3;
	} else if (steps >= FOURTH_OCTANT && steps < FIFTH_OCTANT){
		redValue = steps/2;
	} else if (steps >= FIFTH_OCTANT && steps < SIXTH_OCTANT){
		redValue = steps;
	} else if (steps >= SIXTH_OCTANT && steps < SEVENTH_OCTANT){
		redValue = steps;
	} else { // steps will be between the 7th and 8th octant
		redValue = 0;
	}
	return redValue;
}

unsigned char stepsToGreen (int steps){
	unsigned char greenValue;
	if (steps >= ORIGIN && steps < FIRST_OCTANT){
		greenValue = 0;
	} else if (steps >= FIRST_OCTANT && steps < FOURTH_OCTANT){
		greenValue = steps/2;
	} else if (steps >= FOURTH_OCTANT && steps < FIFTH_OCTANT){
		greenValue = steps + 70;
	} else if (steps >= FIFTH_OCTANT && steps < SEVENTH_OCTANT){
		greenValue = 255 - steps/2;
	} else if (steps >= SEVENTH_OCTANT && steps < REVOLUTION){
		greenValue = 255 - steps;
	} else { // if steps == 256 <==> 0
		greenValue = 0;
	}
	return greenValue;
}

unsigned char stepsToBlue (int steps){
	unsigned char blueValue;

	if (steps >= ORIGIN && steps < FIRST_OCTANT){
		blueValue = steps*2;
	} else if (steps >= FIRST_OCTANT && steps < SECOND_OCTANT){
		blueValue = steps*2;
	} else if (steps >= SECOND_OCTANT && steps < FOURTH_OCTANT){
		blueValue = steps;
	} else if (steps >= FOURTH_OCTANT && steps < FIFTH_OCTANT){
		blueValue = steps + 70;
	} else if (steps >= FIFTH_OCTANT && steps < SEVENTH_OCTANT){
		blueValue = 255 - steps;
	} else if (steps >= SEVENTH_OCTANT && steps < REVOLUTION){
		blueValue = 255 - steps;
	} else { // if steps == 256 <==> 0
		blueValue = 0;
	}
	return blueValue;
}

