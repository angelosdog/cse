// Program to test if a year is a leap year on Mars
// "every odd year or year ending in 0, except every 100th year
// except every 500th year"
// Vincent Tran z3415372 20th Mar 2012

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int isMarsLeapYear (int year);

int main(int argc, char* argv[]) {
    int year;
    
    printf("Enter a year greater than 0\n");
    scanf("%d",&year);

    if (isMarsLeapYear(year) == 0){
       printf("N");
    } else {
       printf("Y");
    }

    return EXIT_SUCCESS;    
}

int isMarsLeapYear (int year){
    int isLeap;
   
    if (year % 2 == 1) {
       isLeap = 1;
    } else if (((year % 10 == 0) && (year % 100 != 0)) || (year % 500 == 0)) {
       isLeap = 1;
    } else {
       isLeap = 0;
    }
	
    return isLeap;
}
