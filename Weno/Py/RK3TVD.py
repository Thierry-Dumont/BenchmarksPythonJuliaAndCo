import numpy as np
c21=3./4.
c22=1./4.
c31=1./3.
c32=2./3.
def RK3TVD(L,InOut,dt):
    size=InOut.shape[0]
    u1=np.empty(size)
    u2=np.empty(size)
    L(InOut,u1)
    u1=InOut + dt*u1

    L(u1,u2)
    u2= c21*InOut+c22*(u1+dt*u2)

    L(u2,u1)
    return c31*InOut+c32*(u2+dt*u1)

