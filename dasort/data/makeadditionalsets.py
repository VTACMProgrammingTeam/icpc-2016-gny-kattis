#!/usr/bin/python

#
# Per is going to reject this if we don't have input sets that maximize
# the problem boundaries.
#
from random import randint

N = 1000

count = 1

def write(v):
    global count
    count += 1
    print "cat > secret%02d.in << EOF" % (count)
    P = 100
    print P
    for K in range(1, P+1):
        print K, N
        assert N % 10 == 0
        for r in range(N/10):
            print " ".join(map(str, v[K-1][r*10:r*10+10]))

    print "EOF"


# random
write([[randint(1, 10**9) for _ in range(N)] for k in range(100)])
# equal values, large
write([[10**9 for _ in range(N)] for k in range(100)])
# equal values, small
write([[1 for _ in range(N)] for k in range(100)])
# alternating values
write([[i % 2 + 1 for i in range(N)] for k in range(100)])
# descending values
write([[(k+1)*10000 - 2 * i for i in range(N)] for k in range(100)])

