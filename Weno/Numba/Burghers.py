from numba import jit
@jit
def flux(x):
    return 0.5*x*x

@jit
def minf(a,b):
    if b<=0:
      return flux(b)
    elif a>=0:
      return flux(a)
    else:
      return 0.0
  
@jit 
def maxf(a,b):
    return max(flux(a),flux(b))
