import numpy as np
from Weno import Weno
from numba import jitclass
from numba import int32, float64,deferred_type

#------------------------------------------------------
Weno_type = deferred_type()
Weno_type.define(Weno.class_type.instance_type)
spec = [
    ('c21', float64), ('c22', float64),
    ('c31', float64), ('c32', float64),
    ('size',int32),
    ('u1', float64[:]),
    ('u2', float64[:]),
    ("W", Weno_type)
]
#-------------------------------------------------------
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
        self.W=Weno(size)

    def op(self,Meth,InOut,dt):
        #Meth(InOut,self.u1)
        #self.W.weno(Meth,self.u2,self.u1)
        #Meth(InOut[0],InOut[1]) #ok
        #a=self.W.weno
        #print(self.W.weno)
        F= lambda x,y: self.W.weno(Meth,x,y)
        pass
