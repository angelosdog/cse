#!/bin/bash

for file in $@
do
    display $file
    echo -n "Address to e-mail this image to? "
    read email
    if [ `echo $email | egrep ^[0-9a-zA-Z]+@[a-z]+[.][a-z]+$` ];
    then
        echo -n "Message to accompany image? "
        read message
        echo "$message" | mutt -s "$file" -a "$file" -- $email
        echo "$file sent to $email"
    fi
done
