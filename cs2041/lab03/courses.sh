#!/bin/bash
IFS='
'

n=0

if [[ $1 = "[A-Z]{4}" ]];
then
    echo "Usage: $0 argument 1 must be a four capital letter sequence" 1>&2
    exit 1
fi

for course in `wget -q -O- "http://www.handbook.unsw.edu.au/vbook2012/brCoursesByAtoZ.jsp?StudyLevel=Undergraduate&descr=$1"| grep "<TD class" | grep $1 | cut -d "<" -f2,3 | cut -d \" -f5 | cut -d "<" -f1 | cut -d ">" -f2`;
do
    if [ $n = 0 ];
    then
        echo -ne "$course"
        n=1
    else 
        echo -ne " $course\n"
        n=0
    fi
done

for course in `wget -q -O- "http://www.handbook.unsw.edu.au/vbook2012/brCoursesByAtoZ.jsp?StudyLevel=Postgraduate&descr=$1"| grep "<TD class" | grep $1 | cut -d "<" -f2,3 | cut -d \" -f5 | cut -d "<" -f1 | cut -d ">" -f2`;
do
    if [ $n = 0 ];
    then
        echo -ne "$course"
        n=1
    else 
        echo -ne " $course\n"
        n=0
    fi
done
