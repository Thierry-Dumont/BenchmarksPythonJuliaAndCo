import numpy as np
import time
import socket
from numba import jit,stencil

def Init(X,L):
    size=X.size
    h=L/size
    for i in range(0,size):
        if i>size//8 and i<size//2+size//8:
            X[i]=1.-2*(i-size//8)*h/L;
        else:
            X[i]=0.0

@jit(nopython=True,nogil=True,parallel=True)
def lapl1d_1(In,Out,niter):
    size=In.size
    h2= (1./size)**2
    for it in range(0,niter):
        Out[1:size-1]= h2*(In[0:size-2]- 2.0*In[1:size-1]+ In[2:size])
        In,Out=Out,In
@jit(nopython=True,nogil=True,parallel=True)
def lapl1d_2(In,Out,niter):
    size=In.size
    h2= (1./size)**2
    for it in range(0,niter):
        for i in range(1,size-1):
            Out[i]=h2*(In[i-1]- 2.0*In[i]+ In[i+1])
        In,Out=Out,In  
@stencil
def kernel(In,h2):
    return h2*(In[1]- 2.0*In[0]+ In[-1])
@jit(nopython=True,nogil=True,parallel=True)
def lapl1d_3(In,Out,niter):
    size=In.size
    h2= (1./size)**2
    for it in range(0,niter):
        kernel(In,h2,out=Out)
    In,Out=Out,In
def test(p,In,Out,nit):
   
    niter=nit
    Init(In,1.)
    Init(Out,1.)
 
    p(In,Out,niter)
    T=0.
    while True:
        Init(In,1.)
        Init(Out,1.)
        t1 = time.time()
        p(In,Out,niter)
        treal=time.time() -t1
        t = treal/niter
        if treal>0.0001 and abs(t-T)/t<0.025:
            break
        else:
            T=t
            niter*=2

    return T,niter

DD={"lapl1d_1":"Vectorized          ",
    "lapl1d_2":"Naive               ",
    "lapl1d_3":"Numba stencil kernel"}

f=open("RunningOn"+socket.gethostname()+"_lapl_1","w")

size=32
sizemax=100000
niter=10
parsef= lambda  f: str(f).split(" ")[1] #parse function name
while size<sizemax:
    print("size: ",size)
    In= np.empty(size)
    Out= np.empty(size)
    tbest=10.**20
    best=0
    t=0.0
    for p in  [lapl1d_1,lapl1d_2,lapl1d_3]:
        t,it=test(p,In,Out,niter)
        if t<tbest:
            tbest=t
            best=p
        print(DD[parsef(p)]," : t= ",t," seconds ")
    nflops= 4*(size-2)
    flops=nflops/tbest
    print("\nbest: ",DD[parsef(best)])
    f.write(str(size)+" "+str(tbest)+"\n")
    print("nb. flops: ",nflops, ", Gflops/s (best): ",flops/(10**9))
    print("-------")
    size*=2
    print(" ")
f.close()
