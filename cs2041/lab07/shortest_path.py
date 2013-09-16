#!/usr/bin/python

import sys, operator
class Road:
    def __init__(self, start, finish, distance):
        self.start = start
        self.finish = finish
        self.distance = distance

class Path:
    def __init__(self, current, previous_towns, distance):
        self.current = current
        self.previous_towns = previous_towns
        self.distance = distance

if len(sys.argv) != 3:
    print >> sys.stderr, "Usage: %s Please supply a start and end point" % sys.argv[0]
    exit()
path = Path(sys.argv[1], [], 0)
next = []
next.append(path)
finish = sys.argv[2]
roads = []
for line in sys.stdin.readlines():
    line.strip()
    line = line.split()
    roads.append(Road(line[0], line[1], int(line[2])))

seen = []
while path.current != finish and len(next) > 0:
    seen.append(path.current)
    path = next.pop(0)
    for road in roads:
        if road.start == path.current and road.finish not in seen:
            prev = path.previous_towns[:]
            prev.append(path.current)
            next.append(Path(road.finish, prev, path.distance + road.distance))
    next.sort(key=operator.attrgetter('distance'))
if len(next) == 0 and path.current != finish:
    sys.stderr.write(sys.argv[0] + ": Couldn't find path between " + sys.argv[1] + " and " + sys.argv[2] + ".")
    exit()
path_to_current = ""
for town in path.previous_towns:
    path_to_current = path_to_current + " " + town    
print "Shortest route is length = " + str(path.distance) + ":" + path_to_current + " " + path.current + "."
