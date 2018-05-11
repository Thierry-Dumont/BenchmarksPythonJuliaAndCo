from numba import jit
import GodunovFlux as Godunov
import Burghers as Burg
from numba import float64
@jit(float64(float64, float64),nopython=True)
def gb(X,Y):
    return Godunov.NumFlux(Burg,X,Y)
