import sys
from collections import Counter

#
# check remaining input properties.
#

B = [map(int, raw_input().split()) for _ in range(8)]

C = Counter(B[i][j] for i in range(8) for j in range(8))

# check that all integers are unique
assert all (number == -1 or count == 1 for number, count in C.iteritems())

# verify that position of 1 is given
assert C[1] == 1

# at most 45 -1 missing positions
assert C[-1] <= 45

#
# Verify that the given partial grid is not inconsistent with being
# a semi-magical grid.
#
where = dict ((B[i][j], (i, j)) for i in range(8) for j in range(8))
for i in range(1, 64):
    if i in where and i+1 in where:
        r0, c0 = where[i]
        r1, c1 = where[i+1]
        assert abs(r0-r1) == 1 and abs(c0-c1) == 2 \
            or abs(r0-r1) == 2 and abs(c0-c1) == 1

# left as an exercise to Per: check that the grid has a solution

sys.exit(42)

