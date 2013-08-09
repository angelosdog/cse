#!/bin/bash
file=`egrep 'B$'`
for string in $file;
do
    n=0
    length=`expr length $string`

    while [ "${string:($length-$n-1):1}" = "B" ];
    do
        n=`expr $n + 1`
    done
    i=$n
    while [ "${string:($length-$n-1):1}" = "A" ];
    do
        i=`expr $i - 1`
        n=`expr $n + 1`
    done
    if test $i -eq 0;
    then
        echo $string
    fi
done

