#!/usr/bin/python

# the original problem did not specify bounds for Bx, Cx, Cy, and L, other than to say that Bx > 0 and Cy > 0.
# it also only had 12 test cases (see secret01.in) but allowed the number of tests cases to be 10000
#
# I introduced a upper bounds for Bx and Cy and upper and lower bounds for Cx.
# In addition, I added a lower and upper bound for L.
#
# In addition, I added the conditions that
# A, B, C form a triangle
#
# L is at least 1 ft longer than the perimeter's triangle
#
# Theoretically, any L greater than the perimeter's triangle should yield
# a solution, but the judge solution complains about invalid input if L is
# too close to the perimeters triangle.
#
# I also added an upper bound for L to be less than 500ft.
#

print "cat > secret02.in << EOF"
P = 10000
eps = 1e-2
Lminslack = 0.05
Lmax = 1000.0

print P
from random import uniform
from math import sqrt

for K in range(1, P+1):
    while True:
        Bx = uniform(eps, 100.0)
        Cx = uniform(-100.0, 100.0)
        Cy = uniform(eps, 100.0)
        # triangle inequality
        s1 = Bx
        s2 = sqrt((Cx-Bx)*(Cx-Bx) + Cy*Cy)
        s3 = sqrt(Cx*Cx + Cy*Cy)
        if s1 + s2 < s3 or s1 + s3 < s2 or s2 + s3 < s1:
            continue

        Lmin = s1 + s2 + s3 + Lminslack
        if not Lmin < Lmax:
            continue

        L = uniform(Lmin, min(3*Lmin, Lmax))
        assert s1 + s2 + s3 + Lminslack <= L
        break

    print "%d %.3f %.3f %.3f %.3f" % (K, Bx, Cx, Cy, L)
print "EOF"
