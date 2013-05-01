// Given the doomsday for a year, and whether or not it is a
// leap year, this function returns the day of the week for any
// given month and day in the year/
// 0 denoting Thursday, 1 Friday ... 6 Wednesday.
// Vincent Tran z3415372 Mar 2012

#include <stdio.h>
#include <stdlib.h>
#include "NextThursday.h"

// Added a lot of #define statements, but I'm a little bit hesitant
// to use them. Read blog for more. 20/3/2012

#define JANUARY 1
#define FEBRUARY 2
#define MARCH 3
#define APRIL 4
#define MAY 5
#define JUNE 6
#define JULY 7
#define AUGUST 8
#define SEPTEMBER 9
#define OCTOBER 10
#define NOVEMBER 11
#define DECEMBER 12

#define DOOMSDAY_JANUARY 3
#define DOOMSDAY_FEBRUARY 0
#define DOOMSDAY_MARCH 0
#define DOOMSDAY_APRIL 4
#define DOOMSDAY_MAY 9
#define DOOMSDAY_JUNE 6
#define DOOMSDAY_JULY 11
#define DOOMSDAY_AUGUST 8
#define DOOMSDAY_SEPTEMBER 5
#define DOOMSDAY_OCTOBER 10
#define DOOMSDAY_NOVEMBER 7
#define DOOMSDAY_DECEMBER 12


int isLeapYear (int year){
    int isLeap;
   
// Revised leap year algorithm to fit in one if statement
// after seeing that it was okay in code review to have a
// line that long - 20/3/2012
    if (((year % 4 == 0) && (year % 100 != 0))||(year % 400 == 0)) {
       isLeap = 1;
    } else {
       isLeap = 0;
    }
	
    return isLeap;
}

int yearToDoomsday (int year){
// Used variables a, b, c and d to match the variables used on wikipedia
// Revised variable names from y, a, b, c and d to lastTwoDigits floor, 
// remainder, remainderDiv4 and total respectively (14/3/2012)

// Last two digits should be the remainder after dividing by 100
    int lastTwoDigits = year%100;
    int floor = lastTwoDigits /12;
    int remainder = lastTwoDigits %12;
    int remainderDiv4 = (lastTwoDigits %12)/4;
    int total = (floor + remainder + remainderDiv4)%7;

    int century = year/100 + 1;
    int anchor = (5 * century + (century - 1)/4 )%7;

    int doomsday = (total + anchor)%7;
	
    return doomsday;
}

int dayOfWeek (int doomsday, int leapYear, int month, int day) {
    int dayIndex = 0;

// After confirming that leapYear will be 1 for true exclusively
// it is more efficient (less if statements) to include subtracting
// leapYear from the day (if it's not a leap year, subtract 0)

// Added #defined constants 20/3/2012

    if (month == JANUARY){
       dayIndex = (day - DOOMSDAY_JANUARY - leapYear + doomsday)%7;
    } else if (month == FEBRUARY){
       dayIndex = (day - DOOMSDAY_FEBRUARY - leapYear + doomsday)%7;
    } else if (month == MARCH){
       dayIndex = (day - DOOMSDAY_MARCH + doomsday)%7;
    } else if (month == APRIL){
       dayIndex = (day - DOOMSDAY_APRIL + doomsday)%7;
    } else if (month == MAY){
       dayIndex = (day - DOOMSDAY_MAY + doomsday)%7;
    } else if (month == JUNE){
       dayIndex = (day - DOOMSDAY_JUNE + doomsday)%7;
    } else if (month == JULY){
       dayIndex = (day - DOOMSDAY_JULY + doomsday)%7;
    } else if (month == AUGUST){
       dayIndex = (day - DOOMSDAY_AUGUST + doomsday)%7;
    } else if (month == SEPTEMBER){
       dayIndex = (day - DOOMSDAY_SEPTEMBER + doomsday)%7;
    } else if (month == OCTOBER){
       dayIndex = (day - DOOMSDAY_OCTOBER + doomsday)%7;
    } else if (month == NOVEMBER){
       dayIndex = (day - DOOMSDAY_NOVEMBER + doomsday)%7;
    } else if (month == DECEMBER){
       dayIndex = (day - DOOMSDAY_DECEMBER + doomsday)%7;
    }

// To account for negative numbers
    if (dayIndex < 0){
      dayIndex = dayIndex + 7;
    }
// Deleted some redundant code after learning that you an still take
// the mod of a negative number; it just returns a negative remainder
// 14/3/2012
    return dayIndex;
}

int daysToNextThursday (int year, int month, int day){
    int doomsdayOfYear;
    int isLeap;
    int currentDay;
    int nextThursday;

    doomsdayOfYear = yearToDoomsday(year);
    isLeap = isLeapYear(year);
    currentDay = dayOfWeek(doomsdayOfYear, isLeap, month, day);

    nextThursday = 7 - currentDay;

    return nextThursday;
}
