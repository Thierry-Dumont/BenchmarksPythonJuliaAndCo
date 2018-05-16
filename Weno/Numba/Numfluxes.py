from numba import jit
import GodunovFlux as Godunov
import LaxFriedrichs as LF
import Burghers as Burg
import Convection

@jit(nopython=True)
def GodunovBurghers(X,Y):
    return Godunov.NumFlux(Burg.minf,Burg.maxf,X,Y)
@jit(nopython=True)
def GodunovConvection(X,Y):
    return Godunov.NumFlux(Convection.minf,Convection.maxf,X,Y)
@jit(nopython=True)
def LaxFriedrichsBurghers(X,Y):
    return LF.NumFlux(Burg.flux,X,Y,1.0)
@jit(nopython=True)
def LaxFriedrichsConvection(X,Y):
    return LF.NumFlux(Convection.flux,X,Y,1.0)
