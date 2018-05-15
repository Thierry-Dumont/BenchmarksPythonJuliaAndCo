from numba import jit,njit,float64
@jit(nopython=True)
def flux(x):
    return 0.5*x*x

@jit(nopython=True)
def minf(a,b):
    if b<=0:
      return flux(b)
    elif a>=0:
      return flux(a)
    else:
      return 0.0
  
@jit(nopython=True)
def maxf(a,b):
    return max(flux(a),flux(b))
