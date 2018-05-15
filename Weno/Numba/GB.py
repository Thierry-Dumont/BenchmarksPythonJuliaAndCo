from numba import jit,float64,njit
import GodunovFlux as Godunov
import Burghers as Burg

@jit(nopython=True)
def gb(X,Y):
    return Godunov.NumFlux(Burg,X,Y)
