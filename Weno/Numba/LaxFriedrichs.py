from numba import jit
@jit(nopython=True)
def NumFlux(F,a,b,alpha):
    return 0.5*(F(a)+F(b) - alpha*(b-a))
