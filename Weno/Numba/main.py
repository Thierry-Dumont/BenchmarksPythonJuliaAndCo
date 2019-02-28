import numpy as np
from Weno import *
import Numfluxes as NF
from  RK3TVD import *
import numba as nb
import time
import socket

size=1000
L=1.0
dt=0.8/size
T=1.
def init(X):
     h=L/size
     for i in range(0,size):
         if i>size//8 and i<size//2+size//8:
             X[i]=1.-2*(i-size//8)*h/L;
         else:
             X[i]=0.0

#
In = np.empty(size)
Out = np.empty(size)
init(In)

np.savetxt("gp0",In)

print("size= ",size," dt= ",dt," nteps=", T/dt)

R=RK3TVD(size,L)

#NumF=NF.GodunovConvection
NumF=NF.GodunovBurghers
#NumF=NF.LaxFriedrichsConvection
#NumF=NF.LaxFriedrichsBurghers

if NumF==NF.GodunovBurghers:
     pE="Burghers"
     pF="Godunov"
elif NumF==NF.GodunovConvection:
     pE="Convection"
     pF="Godunov"
elif NumF==NF.LaxFriedrichsConvection:
     pE="Convection"
     pF="Lax-Friedrichs"
else:
     pE="Convection"
     pF="Burghers"
t=0

print(pE," with ",pF)
t1 = time.time()
while t<T:
     Out=R.op(NumF,In,dt)
     In,Out=Out,In
     t+=dt

t=(time.time()-t1)
print("computing time: ",t)
fi=open("gp","w")
np.savetxt("gp",In)
fi.close()
print("A file 'gp' with the final solution was created.")

f=open("RunningOn"+socket.gethostname(),"w")
f.write(pE+" "+pF+"\n")
f.write(str(t)+"\n")
f.close()
