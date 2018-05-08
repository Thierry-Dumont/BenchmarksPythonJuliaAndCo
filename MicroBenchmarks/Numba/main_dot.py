import numpy as np
import time
from numba import jit

def Init(X,L):
    size=X.size
    h=L/size
    for i in range(0,size):
        if i>size//8 and i<size//2+size//8:
            X[i]=1.-2*(i-size//8)*h/L;
        else:
            X[i]=0.0

@jit
def dotd_1(X,Y,niter):
    size=X.size
    for it in range(0,niter):
        s=0.0
        X[1]=it
        for i in range(0,size):
            s+=X[i]*Y[i]
        #X*=0.8
@jit
def dotd_2(X,Y,niter):
    for it in range(0,niter):
        X[1]=it
        s= X.dot(Y)
        #X*=0.8

def test(p,X,Y,nit):
   
    niter=nit
    
    T=0.
    while True:
        
        Init(X,1.)
        Init(Y,1.)
        t1 = time.time()
        p(X,Y,niter)
        t=time.time() -t1
        if abs(t-2.*T)/t<0.01:
            break
        else:
            T=t
            niter*=2
    #print(T,niter,T/niter)
    return T/niter,niter

size=100
sizemax=10**6
niter=4
parsef= lambda  f: str(f).split(" ")[1] #parse function name
while size<sizemax:
    print("size: ",size)
    X= np.empty(size)
    Y= np.empty(size)
    tbest=10.**20
    best=0
    t=0.0
    for p in  [dotd_1,dotd_2]:
        t,it=test(p,X,Y,niter)
        if t<tbest:
            tbest=t
            best=p
        print(parsef(p)," : t= ",t," seconds ")
    nflops=size*2
    print("tbest",tbest,X.size)
    flops=nflops/tbest
    print("\nbest: ",parsef(best))
    print("nb. flops (best): ",nflops, ", Gflops/s: ",flops/(10**9))
    print("-------")
    size*=2
    print(" ")
