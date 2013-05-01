// Program to test if a year is a leap year,
// provided year >= 1582
// Testing -4, 0, 1581, 1582, 1583, 1900, 2000, 2011, 2012
// Vincent Tran z3415372 19th  Mar 2012

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int isLeapYear (int year);

int main(int argc, char* argv[]) {

    int year;
    
    printf("Enter a year after 1582\n");
    assert(scanf("%d", &year) == 1 );
    assert(year >= 1582);

    if (isLeapYear(year) == 0){
       printf("%d is not a leap year\n", year);
    } else {
       printf("%d is a leap year\n", year);
    }

    return EXIT_SUCCESS;    
}

int isLeapYear (int year){
    int isLeap;
   
    if (year % 4 == 0) {
       if (year % 100 == 0) {
          if (year % 400 == 0) {
             isLeap = 1;
           } else {	
              isLeap = 0;
           }
        } else {
           isLeap = 1;
        }
    } else {
       isLeap = 0;
    }
	
    return isLeap;
}
