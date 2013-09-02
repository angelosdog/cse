#!/usr/bin/python

import sys

for i in range(1, len(sys.argv)):
    tail = []
    for j in range(0,10):
        tail.append('')
    f = open(sys.argv[i], 'r')
    for line in f:
        tail.append(line)
        tail.pop(0)
    sys.stdout.write("".join(tail))
