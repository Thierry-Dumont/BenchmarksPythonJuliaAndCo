from numba import jit,njit,float64
@jit(nopython=True)
def NumFlux(F,a,b):
    if a<=b:
      return F.minf(a,b)
    else:
      return F.maxf(b,a);
