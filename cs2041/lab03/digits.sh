#!/bin/bash

while read line;
do
    line=`echo $line | sed -e "s/[0-4]/</g"`
    line=`echo $line | sed -e "s/[6-9]/>/g"`
    echo $line
done
