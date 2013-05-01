//
// NextThursday.h
// Version 1.1
// task1 Computing 1: comp1917 2012s1
// OpenLearning UNSW Computing
//
// Created by Richard Buckland on 12/03/11.
// Copyright 2011
// Licensed under Creative Commons SA-BY-NC 3.0.
//

#define START_GREGORIAN_CALENDAR 1582

// given the doomsday for a year, and whether or not it is a
// leap year, this function return the day of the week for any
// given month and day in the year.
// 0 denoting Thursday, 1 Friday ... 6 Wednesday.
int dayOfWeek (int doomsday, int leapYear, int month, int day);

// given a year > START_GREGORIAN_CALENDAR this function returns
// 1 if it is a leap year and 0 otherwise
int isLeapYear (int year);

// given a year > START_GREGORIAN_CALENDAR this function returns
// the doomsday for that year (a number in the range 0..6)
// 0 denoting Thursday, 1 Friday ... 6 Wednesday.
int yearToDoomsday (int year);

// given a date after START_GREGORIAN_CALENDAR this function returns
// the number of days until the next thursday.
// eg for 16 March 2011 (2011,3,16)  the function will return 1
// eg for 17 March 2011 (2011,3,17)  the function will return 7
int daysToNextThursday (int year, int month, int day);
