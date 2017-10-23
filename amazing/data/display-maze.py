#!/usr/bin/python

#
# Display the maze. I used this to create the .desc files.
#
_, w, h, hassolution = raw_input().split()
assert _ == "start"
w = int(w)
h = int(h)

WALL_DOWN = -1
wALL_MUST_BE_UP = 0
wALL_UP = 1

#   _____
#  |_|_|_|
#  |_|_|_|
#
#
print "hassolution: ", hassolution

print " %s" % ("_" * (2*w-1))

D = []
R = []
for i in range(h):
    row = raw_input()
    D.append([])
    R.append([])

    for l in row:
        val = int(l, 16)
        d = (val >> 2) - 1
        assert d != 2
        D[i].append(d)
        r = (val & 0x3) - 1
        assert r != 2
        R[i].append(r)

for i in range(h):
    out = "|"
    for j in range(w):
        if D[i][j] == WALL_DOWN:
            out += " "
        else:
            out += "_"

        if R[i][j] == WALL_DOWN:
            if i == h - 1:
                out += "_"
            elif R[i+1][j] == WALL_DOWN:
                if D[i][j] == WALL_DOWN and D[i][j+1] == WALL_DOWN:
                    out += " "
                else:
                    out += "_"
            else:
                out += " "
        else:
            out += "|"

    print out

