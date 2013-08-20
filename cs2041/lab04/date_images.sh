#!/bin/bash

for file in $@
do
    time=`ls -l "$file" | cut -d\  -f6,7,8`
    convert -gravity south -pointsize 36 -annotate 0  "$time" "$file" "$file"
    touch -d "$time" "$file"
done
