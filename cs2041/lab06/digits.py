#!/usr/bin/python
import sys, re

for line in sys.stdin.readlines():
    line = re.sub("[0-4]", "<", line)
    line = re.sub("[6-9]", ">", line)
    print line
