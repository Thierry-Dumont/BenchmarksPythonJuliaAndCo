import numpy as np
from numba import jitclass
from numba import int32, float64,void

spec = [
    ('c21', float64), ('c22', float64),
    ('c31', float64), ('c32', float64),
    ('size',int32),
    ('u1', float64[:]),
    ('u2', float64[:])
    
]
@jitclass(spec)
class RK3TVD:
    def __init__(self,size):
        self.c21=3./4.
        self.c22=1./4.
        self.c31=1./3.
        self.c32=2./3.
        self.size=size
        self.u1=np.empty(self.size)
        self.u2=np.empty(self.size)

    def op(self,Meth,X,dt):
        pass


