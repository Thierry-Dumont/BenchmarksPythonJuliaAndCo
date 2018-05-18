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
def cl_1(A,B,C,D,niter):
    size=A.size
    for it in range(0,niter):
        A=1.7*B-0.8*C-0.9*D
        A,D=D,A
@jit
def cl_2(A,B,C,D,niter):
    size=A.size
    for it in range(0,niter):
        for i in range(1,size-1):
            A[i]=1.7*B[i]-0.8*C[i]-0.9*D[i]
        A,D=D,A    
              
def test(p,A,B,C,D,nit):
   
    niter=nit
    Init(A,1.)
    Init(B,1.)
    Init(C,1.)
    Init(D,1.)
    p(A,B,C,D,niter)
    T=0.
    while True:
        Init(A,1.)
        Init(B,1.)
        Init(C,1.)
        Init(D,1.)
        t1 = time.time()
        p(A,B,C,D,niter)
        treal=time.time() -t1
        t = treal/niter
        if treal>0.0001 and abs(t-T)/t<0.025:
            break
        else:
            T=t
            niter*=2

    return T,niter

DD={"cl_2":"Naïve     ",
    "cl_1":"Vectorized"}
size=16
sizemax=100000
niter=10
parsef= lambda  f: str(f).split(" ")[1] #parse function name
while size<sizemax:
    print("size: ",size)
    A= np.empty(size)
    B= np.empty(size)
    C= np.empty(size)
    D= np.empty(size)
    tbest=10.**20
    best=0
    t=0.0
    for p in  [cl_1,cl_2]:
        t,it=test(p,A,B,C,D,niter)
        if t<tbest:
            tbest=t
            best=p
        print(DD[parsef(p)]," : t= ",t," seconds ")
    nflops= 4*(size-2)
    flops=nflops/tbest
    print("\nbest: ",DD[parsef(best)])
    print("nb. flops: ",nflops, ", Gflops/s (best): ",flops/(10**9))
    print("-------")
    size*=2
    print(" ")
