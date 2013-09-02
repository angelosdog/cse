#!/bin/bash
echo "Running /usr/bin/paste -s $@"
echo "Running ./paste.pl $@"
echo "Running diff on the output"

if diff <(/usr/bin/paste -s $@) <(./pastes.pl $@)
then
    echo "Test succeeded - output of ./pastes.pl /usr/bin/paste"
else
    echo "Test failed - output of ./pastes.pl is different to /usr/bin/paste"
fi

