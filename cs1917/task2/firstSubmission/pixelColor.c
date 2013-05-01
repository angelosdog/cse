#include "pixelColor.h"
#include <math.h>
#define MAX_ITERATIONS 256

unsigned char stepsToRed (int steps){
	unsigned char redValue;
	if (steps >= 0 && steps < 32){
		redValue = 0;
	} else if (steps >= 32 && steps < 64){
		redValue = steps/5;
	} else if (steps >= 64 && steps < 96){
		redValue = steps/4;
	} else if (steps >= 96 && steps < 128){
		redValue = steps/3;
	} else if (steps >= 128 && steps < 160){
		redValue = steps/2;
	} else if (steps >= 160 && steps < 224){
		redValue = steps;
	} else if (steps >= 224 && steps < 256){
		redValue = steps;
	} else {
		redValue = 0;
	}
	return redValue;
}

unsigned char stepsToGreen (int steps){
	unsigned char greenValue;
	if (steps >= 0 && steps < 32){
		greenValue = 0;
	} else if (steps >= 32 && steps < 128){
		greenValue = steps/2;
	} else if (steps >= 128 && steps < 160){
		greenValue = steps + 70;
	} else if (steps >= 160 && steps < 224){
		greenValue = 255 - steps/2;
	} else if (steps >= 224 && steps < 256){
		greenValue = 255 - steps;
	} else {
		greenValue = 0;
	}
	return greenValue;
}

unsigned char stepsToBlue (int steps){
	unsigned char blueValue;

	if (steps >= 0 && steps < 32){
		blueValue = steps*2;
	} else if (steps >= 32 && steps < 64){
		blueValue = steps*2;
	} else if (steps >= 64 && steps < 128){
		blueValue = steps;
	} else if (steps >= 128 && steps < 160){
		blueValue = steps + 70;
	} else if (steps >= 160 && steps < 224){
		blueValue = 255 - steps;
	} else if (steps >= 224 && steps < 256){
		blueValue = 255 - steps;
	} else {
		blueValue = 0;
	}
	return blueValue;
}

