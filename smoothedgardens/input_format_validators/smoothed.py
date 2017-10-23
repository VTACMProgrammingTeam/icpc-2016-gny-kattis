#!/usr/bin/python

import sys, re

# supplementary input checker.
# adds checks beyond what's check in the .viva file

def check():
    ps = raw_input().split()[1:]

    # via regexp
    # allows
    # 0, 1, .0, .00, .000, 0.0, 0.00, 0.000, 0.0000
    # also matches "" but that would fail other tests.

    def atmost3digits(ps):
        return all(re.match(r'^-?(\d*?(\.\d{0,3})?)$', v) != None for v in ps)

    assert atmost3digits(ps)

P = int(raw_input())

for K in range(P):
    check()

sys.exit(42)

