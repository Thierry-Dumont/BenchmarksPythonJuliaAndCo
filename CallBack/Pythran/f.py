from math import exp
#pythran export capsule f(float)
def f(x):
    return exp(-x)*x*x
