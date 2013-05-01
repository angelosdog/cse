// Program determines what day the doomsday
// is given the year.
// Vincent Tran, z3415372, 9th Mar 2012

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char* argv[]) {
// Using the variables on the wikipedia page; a, b, and c
//  for lack of better variable names.
	int year = 2004;
// Last two digits should be the remainder after dividing by 100
	int y = year%100;
	int a = y/12;
	int b = y%12;
	int c = (y%12)/4;
	int d = (a + b + c)%7;
	int century = year/100 + 1;
	int anchor = (5 * century + (century - 1)/4 )%7;
	int doomsday = (d + anchor)%7;

	if (doomsday == 0){
	 printf("The doomsday for %d is Thursday!\n", year);
	}
	if (doomsday == 1){
	 printf("The doomsday for %d is Friday!\n", year);
	}
	if (doomsday == 2){
	 printf("The doomsday for %d is Saturday!\n", year);
	}
	if (doomsday == 3){
	 printf("The doomsday for %d is Sunday!\n", year);
	}
	if (doomsday == 4){
	 printf("The doomsday for %d is Monday!\n", year);
	}
	if (doomsday == 5){
	 printf("The doomsday for %d is Tuesday!\n", year);
	}
	if (doomsday == 6){
	 printf("The doomsday for %d is Wednesday!\n", year);
	}

 return 0;
}
