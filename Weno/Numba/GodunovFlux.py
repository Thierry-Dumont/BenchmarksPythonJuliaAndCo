from numba import jit
@jit
def NumFlux(F,a,b):
    if a<=b:
      return F.minf(a,b)
    else:
      return F.maxf(b,a);
