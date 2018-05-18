from numba import jit
@jit(nopython=True)
def NumFlux(Fmin,Fmax,a,b):
    if a<=b:
      return Fmin(a,b)
    else:
      return Fmax(b,a);
