from numba import jit
@jit(nopython=True)
def flux(x):
    return x
@jit(nopython=True)
def minf(a,b):
    if b<=0:
      return b
    elif a>=0:
      return a
    else:
      return 0.0
@jit(nopython=True)  
def maxf(a,b):
    return max(a,b)
