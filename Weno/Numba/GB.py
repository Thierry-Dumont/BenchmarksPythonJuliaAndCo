from numba import jit
import GodunovFlux as Godunov
import Burghers as Burg
@jit
def gb(X,Y):
    return Godunov.NumFlux(Burg,X,Y)
