#!/usr/bin/python
#
# There may be multiple correct answers. Your result will be graded correct if it is a semi-magical
# knight's tour and the positive labels from the input are in the same square in your answer.
#
import sys

inputfile = sys.argv[1]
judgeanswer = sys.argv[2]
# feedbackdir = sys.argv[3]

B = []

with open(inputfile) as ifile:
    for line in ifile:
        B.append(map(int, line.split()))

def success():
    sys.exit(42)

def wronganswer():
    sys.exit(43)

try:
    T = []  # team answer
    for _ in range(8):
        T.append(map(int, raw_input().split()))

    if len(T) != 8 or not all(len(t) == 8 for t in T):
        wronganswer()

    for i in range(8):
        for j in range(8):
            if not (1 <= T[i][j] <= 64):
                wronganswer()

            if B[i][j] != -1 and B[i][j] != T[i][j]:
                wronganswer()

    where = dict ((T[i][j], (i, j)) for i in range(8) for j in range(8))
    assert len(where) == 64

    for i in range(1, 64):
        r0, c0 = where[i]
        r1, c1 = where[i+1]
        assert abs(r0-r1) == 1 and abs(c0-c1) == 2 \
            or abs(r0-r1) == 2 and abs(c0-c1) == 1

    assert all(sum(t) == sum(T[0]) for t in T)

    firstColSum = sum(T[i][0] for i in range(8))
    assert all(firstColSum == sum(T[i][j] for i in range(8)) for j in range(8))

    success()

except Exception as e:
    print >>sys.stderr, "Exception: %s" % (str(e))

wronganswer()

