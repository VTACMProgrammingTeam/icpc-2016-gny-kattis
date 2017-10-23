#!/usr/bin/python

#
# I bet Per will want a validator that a maze that says "hasSolution"
# has indeed a solution.
#
import sys

_, w, h, hasSolution = raw_input().split()
w = int(w)
h = int(h)

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

WALL_DOWN = -1

# the initial position is (0, 0)
dfs = [(0, 0)]
seen = set((dfs[0],))
solved = False

while len(dfs) > 0:
    r, c = dfs.pop()
    if r >= h or r < 0 or c >= w or c < 0:    # anything outside.  However, in the provided tests, 
                                            # all exits are at the bottom right.
        solved = True
        break

    def schedule(r, c):
        if (r, c) not in seen:
            seen.add((r, c))
            dfs.append((r, c))

    if c < w and R[r][c] == WALL_DOWN:      # RIGHT
        schedule(r, c+1)
    if c > 0 and R[r][c-1] == WALL_DOWN:    # LEFT
        schedule(r, c-1)
    if r < h and D[r][c] == WALL_DOWN:      # DOWN
        schedule(r+1, c)
    if r > 0 and D[r-1][c] == WALL_DOWN:    # UP
        schedule(r-1, c)

assert (solved and hasSolution == "1") or (not solved and hasSolution == "0")
sys.exit(42) 
