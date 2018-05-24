from numba import jit
@jit(nopython=True)
def flux(x):
    return 0.5*x*x

@jit(nopython=True)
def minf(a,b):
    if b<=0:
      return  0.5*b*b
    elif a>=0:
      return 0.5*a*a
    else:
      return 0.0
  
@jit(nopython=True)
def maxf(a,b):
    return 0.5*max(a*a,b*b)
