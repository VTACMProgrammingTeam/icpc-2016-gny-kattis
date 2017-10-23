import sys

#
# This checks that "it fits in a 32-unsigned integer"
#
# Godmar's naive solution
#
def memoize(f):
    cache= {}
    def memf(*x):
        if x not in cache:
            cache[x] = f(*x)
        return cache[x]
    return memf

import sys
sys.setrecursionlimit(12000)

def R(): return map(int, raw_input().split())

P = R()[0]
for K in range(1, P+1):
    k, m, n = R()

    @memoize
    def solve(n, maxK = 0):
        """
        n = m^k + m^k ... + m^k + m^{k-1} + ... + 1
        """
        # print m, n, maxK
        if maxK == 0:
            return 1

        p = m**maxK
        answer = solve(n, maxK - 1)
        if n >= p:
            answer += solve(n - p, maxK)

        return answer

    p = 0
    while m**p <= n:
        p += 1

    answer = solve(n, p-1)
    assert 0 <= answer < 2**32      # <<<<----------------------

sys.exit(42)

