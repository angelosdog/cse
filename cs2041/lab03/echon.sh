#!/bin/bash

if [ $# -lt 2 ];
then
    echo "Usage: ./echon.sh not enough arguments" 1>&2
    exit 1
elif [ $# -gt 2 ];
then
    echo "Usage: ./echon.sh too many arguments" 1>&2
    exit 1
elif [[ $1 = *[!0-9]* ]];
then
    echo "Usage: ./echon.sh argument 1 must be a non-negative integer" 1>&2
    exit 1
elif [ $1 -lt 0 ];
then
    echo "Usage: ./echon.sh argument 1 must be a non-negative integer" 1>&2
    exit 1
fi

n=0

while [ $n -lt $1 ];
do
    echo $2
    n=`expr $n + 1`
done
