#!/usr/bin/env python3

import sys

def gcd_ex(m, n):
        """Returns a result of extended euclidian algorithm.
        
        This function takes two arguments (m, n), and calculates one of the answers of
        the following equation:
                x m + y n = gcd(m, n)
        and returns a tuple (x, y, gcd(m, n)).
        """
        (a0, b0, a1, b1) = (1, 0, 0, 1) if m >= n else (0, 1, 1, 0)
        if m < n:
                (m, n) = (n, m)
        while n > 0:
                q = m // n
                (a0, b0, a1, b1) = (a1, b1, a0 - q * a1, b0 - q * b1)
                (m, n) = (n, m % n)
        return (a0, b0, m)

if len(sys.argv) < 3:
    print("Usage: python3 gcd_ex.py a b")
    sys.exit(1)

a = int(sys.argv[1])
b = int(sys.argv[2])

r = gcd_ex(a, b)

print("{2} * {0} + {3} * {1} = {4}".format(a, b, r[0], r[1], r[2]))


