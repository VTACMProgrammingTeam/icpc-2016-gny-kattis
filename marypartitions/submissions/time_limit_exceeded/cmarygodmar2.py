#!/usr/bin/python

def R(): return map(int, raw_input().split())

P = R()[0]
for K in range(1, P+1):
    k, m, n = R()

    def solve(n, maxK = 0):
        """
        n = m^k + m^k ... + m^k + m^{k-1} + ... + 1
        """
        # print m, n, maxK
        if maxK == 0:
            return 1

        if maxK == 1:
            return n/m + 1

        p = m**maxK
        return sum(solve(n - i*p, maxK - 1) for i in range(n/p + 1))

    p = 0
    while m**p <= n:
        p += 1

    print k, solve(n, p-1)
