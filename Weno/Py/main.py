import numpy as np
from Weno import *
import cProfile
import time
import socket
import GodunovFlux as Godunov
import LaxFriedrichs 
import Burghers as Burg
import Convection
from  RK3TVD import *
import time

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


W=Weno(size)

# Choose on equation:
Flux=Burg
#Flux=Convection

# Choose one numerical flux:
NumFlux=Godunov
#NumFlux=LaxFriedrichs 

if NumFlux==Godunov:
     pF="Godunov"
else:
     pF="Lax-Friedrichs"
if Flux==Burg:
     pE="Burghers"
else:
     pE="Convection"
     
Meth=lambda x,y: W.weno(NumFlux,Flux,L,x,y)


R=RK3TVD(size)

print(pE," with ",pF)

t=0

t1 = time.time()
while t<T:
     Out=R.op(Meth,In,dt)
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
