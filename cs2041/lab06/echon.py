#!/usr/bin/python

import sys, re

if len(sys.argv) == 1:
    sys.stderr.write("Usage: " + sys.argv[0] + " Not enough arguments")
    exit()
elif len(sys.argv) > 3:
    sys.stderr.write("Usage: " + sys.argv[0] + " Too many arguments")
    exit()
elif not re.match(r'^[0-9]+$', sys.argv[1]):
    sys.stderr.write("UsageL " + sys.argv[0] + " First argument must be a number")
    exit()

for i in range(0, int(sys.argv[1])):
    print sys.argv[2]
