import numpy as np
from Weno import *
import time
import socket
import GodunovFlux as Godunov
import Burghers as Burg
from  RK3TVD import *
import time

size=500
L=1.0
dt=0.8/size
T=3.
def init(X):
     h=L/size
     for i in range(0,size):
         if i>size//8 and i<size//2+size//8:
             X[i]=1.-2*(i-size//8)*h/L;
         else:
             X[i]=0.0

#
In = np.empty(size)
init(In)
#print(In)
np.savetxt("gp0",In)

print("size= ",size," dt= ",dt," nteps=", T/dt)

Meth=lambda x,y: Weno(Godunov,Burg,L,x,y)
t=0

t1 = time.time()
while t<T:
     Out=RK3TVD(Meth,In,dt)
     In,Out=Out,In
     #print(In-Out)
     t+=dt
     #print("t",t,T)
t=(time.time()-t1)
print("computing time: ",t)
# fi=open("gp","w")
np.savetxt("gp",In)
# fi.close()
#print(M)
#weno.Weno(Godunov,Burg,L,In,Out)
# f=open("RunningOn"+socket.gethostname(),"w")   
# for x in sorted(D.keys()):
#     f.write(str(x)+" "+str(D[x])+"\n")
# f.close()
