#!/bin/bash
IFS=$(echo -en "\n\b")

for file in *;
do
    if [ `echo $file | grep jpg$` ];
    then
        newFile=`echo $file | sed 's/jpg$//'`png
        if [ ! -e "$newFile" ];
        then
            convert $file $newFile
            rm $file
        else
            echo Usage: $0 File already exists 1>&2
        fi
    fi
done
