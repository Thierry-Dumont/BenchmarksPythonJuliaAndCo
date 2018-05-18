import numpy as np
import time
from lapl2d_1 import *
from lapl2d_2 import *

def Init(X,L):
    size=X.shape[0]
    h=L/size
    for i in range(0,size):
        for j in range(0,size):
            if i>size//8 and i<size//2+size//8:
                X[i]=1.-2*(i-size//8)*h/L;
            else:
                X[i]=0.0

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

size=16
sizemax=1025
niter=10
parsef= lambda  f: str(f).split(" ")[2][:-1] #parse function name

DD={"lapl2d_1":"Vectorized",
    "lapl2d_2":"Naïve     "}
while size<sizemax:
    print("size: ",size)
    In= np.empty((size,size))
    Out= np.empty((size,size))
    tbest=10.**20
    best=0
    t=0.0
    for p in  [lapl2d_1,lapl2d_2]:
        t,it=test(p,In,Out,niter)
        if t<tbest:
            tbest=t
            best=p
        print(DD[parsef(p)]," : t= ",t," seconds ")
    nflops=6*(size-2)**2
    flops=nflops/tbest
    print("\nbest: ",DD[parsef(best)])
    print("nb. flops: ",nflops, ", Gflops/s (best): ",flops/(10**9))
    print("-------")
    size*=2
    print(" ")
