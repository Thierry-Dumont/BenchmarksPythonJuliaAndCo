from math import exp
#pythran export f(float)
def f(x):
    return exp(-x)*x*x
