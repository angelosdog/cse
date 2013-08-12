#!/bin/bash

smallIndex=0
medIndex=0
largeIndex=0

for file in `ls -X`;
do
    if [ `cat $file | wc -l` -lt 10 ];
    then
        small[smallIndex]=$file
        smallIndex=`expr $smallIndex + 1`
    elif [ `cat $file | wc -l` -ge 100 ];
    then
        large[largeIndex]=$file
        largeIndex=`expr $largeIndex + 1`
    else
        med[medIndex]=$file
        medIndex=`expr $medIndex + 1`
    fi
done

echo -ne "Small files:"
for i in $(seq 0 $smallIndex);
do
    echo -ne " ${small[$i]}"
done
echo -ne "\n"

echo -ne "Medium-sized files:"
for i in $(seq 0 $medIndex);
do
    echo -ne " ${med[$i]}"
done
echo -ne "\n"

echo -ne "Large files:"
for i in $(seq 0 $largeIndex);
do
    echo -ne " ${large[$i]}"
done
echo -ne "\n"

