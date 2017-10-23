#!/usr/bin/python

K = int(raw_input())

for c in range(1, K+1):
    raw_input()

    print "cat > secret%03d.in << EOF" % (c)
    for i in range(6):
        print raw_input()
    print "EOF"

