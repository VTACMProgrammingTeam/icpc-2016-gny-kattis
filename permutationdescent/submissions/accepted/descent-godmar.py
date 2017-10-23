#!/usr/bin/python
import sys
sys.setrecursionlimit(10000)

def memoize(f):
    cache= {}
    def memf(*x):
        if x not in cache:
            cache[x] = f(*x)
        return cache[x]
    return memf

@memoize
def PDC(n, v):
    return 1 if v == 0 or v == n - 1 else (v + 1) * PDC(n - 1, v) + (n - v) * PDC(n - 1, v - 1)

T = int(raw_input())
for _ in range(T):
    k, n, v = map(int, raw_input().split())
    print k, PDC(n, v) % 1001113
