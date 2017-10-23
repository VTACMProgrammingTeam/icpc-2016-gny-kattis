import sys

#
# This checks that "neither nominator nor denominator will overflow a 32-unsigned integer"
#

from fractions import Fraction as F

def R(): return map(int, raw_input().split())

def solve(n):
    if n == 1:
        return F(1)
    else:
        h = solve(n/2)
        if n & 1 == 1:
            return F(h.numerator + h.denominator, h.denominator)
        else:
            return F(h.numerator, h.numerator + h.denominator)
    

P = R()[0]
for K in range(1, P+1):
    k, n = R()
    f = solve(n)
    assert 0 <= f.numerator < 2**32        # <<<<----------------------
    assert 0 <= f.denominator < 2**32      # <<<<----------------------

sys.exit(42)

