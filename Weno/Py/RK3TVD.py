import numpy as np
class RK3TVD:
    def __init__(self,size):
        self.c21=3./4.
        self.c22=1./4.
        self.c31=1./3.
        self.c32=2./3.
        self.size=size
        self.u1=np.empty(self.size)
        self.u2=np.empty(self.size)
    def op(self,Meth,InOut,dt):
        Meth(InOut,self.u1)
        self.u1=InOut + dt*self.u1

        Meth(self.u1,self.u2)
        self.u2= self.c21*InOut+self.c22*(self.u1+dt*self.u2)

        Meth(self.u2,self.u1)
        return self.c31*InOut+self.c32*(self.u2+dt*self.u1)

