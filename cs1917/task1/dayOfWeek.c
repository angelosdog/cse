// Given the doomsday for a year, and whether or not it is a
// leap year, this function returns the day of the week for any
// given month and day in the year/
// 0 denoting Thursday, 1 Friday ... 6 Wednesday.
// Vincent Tran z3415372 Mar 2012

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int doomsday = 6;
	int leapYear = 1;
	int month = 2;
	int day = 12;
	int dayIndex;

// After confirming that leapYear will be 1 for true exclusively
// it is more efficient (less if statements) to include subtracting
// leapYear from the day (if it's not a leap year, subtract 0)

    if (month == 1){
	  dayIndex = (day - 3 - leapYear + doomsday)%7;
	} else if (month == 2){
	  dayIndex = (day - 0 - leapYear + doomsday)%7;
	} else if (month == 3){
	  dayIndex = (day - 0 + doomsday)%7;
	} else if (month == 4){
	  dayIndex = (day - 4 + doomsday)%7;
	} else if (month == 5){
	  dayIndex = (day - 9 + doomsday)%7;
	} else if (month == 6){
	  dayIndex = (day - 6 + doomsday)%7;
	} else if (month == 7){
	  dayIndex = (day - 11 + doomsday)%7;
	} else if (month == 8){
	  dayIndex = (day - 8 + doomsday)%7;
	} else if (month == 9){
	  dayIndex = (day - 5 + doomsday)%7;
	} else if (month == 10){
	  dayIndex = (day - 10 + doomsday)%7;
	} else if (month == 11){
	  dayIndex = (day - 7 + doomsday)%7;
	} else if (month == 12){
	  dayIndex = (day - 12 + doomsday)%7;
	}


	if (dayIndex < 0){
	dayIndex = dayIndex + 7;
	}
// To account for the 1st and 2nd of Oct which will still give negative
// dayIndex's
	if (dayIndex < 0){
	dayIndex = dayIndex + 7;
	}
	dayIndex = dayIndex%7;

// Convert dayIndex to print an actual string

	if(dayIndex == 0){
	 printf("It's a Thursday!");
	}						
	if(dayIndex == 1){
	 printf("It's a Friday!");
	}
	if(dayIndex == 2){
	 printf("It's a Saturday!");
	}
	if(dayIndex == 3){
	 printf("It's a Sunday!");
	}
	if(dayIndex == 4){
	 printf("It's a Monday!");
	}
	if(dayIndex == 5){
	 printf("It's a Tuesday!");
	}if(dayIndex == 6){
	 printf("It's a Wednesday!");
	}
  return EXIT_SUCCESS;
}
