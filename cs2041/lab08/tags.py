#!/usr/bin/python
import sys, re, subprocess, operator
if sys.argv[1] == "-f":
    f = True
    p = subprocess.Popen(["wget","-q","-O-",sys.argv[2]], stdout=subprocess.PIPE)
else:
    f = False
    p = subprocess.Popen(["wget","-q","-O-",sys.argv[1]], stdout=subprocess.PIPE)

tags = {}
for line in iter(p.stdout.readline, ""):
    line = re.sub(r'&\w+;', ' ', line)
    if not re.match(r'^\s+$', line):
        line = line.strip()
        split = re.split('(<.*?>)', line)
        html = []
        for i in range(1, len(split), 2):
            html.append(split[i])
        for tag in html:
            tag = tag.replace("/", "")
            if not re.match(r'<!--', tag) and re.match(r'<[a-z]', tag):
                tag = re.split(r'^<([a-z]+)[^a-z]', tag)[1]
                if tag not in tags:
                    tags[tag] = 1
                else:
                    tags[tag] += 1

if f == True:
    for key in sorted(tags, key=tags.get):
        print key + " " + str(tags[key])
else:
    for key in sorted(tags.iterkeys()):
        print key + " " + str(tags[key])
