import numpy as np
import time

def Init(X,L):
    size=X.size
    h=L/size
    for i in range(0,size):
        if i>size//8 and i<size//2+size//8:
            X[i]=1.-2*(i-size//8)*h/L;
        else:
            X[i]=0.0


def dotd_1(X,Y,niter):
    size=X.size
    for it in range(0,niter):
        s=0.0
        for i in range(0,size):
            s+=X[i]*Y[i]
        X,Y=Y,X

def dotd_2(X,Y,niter):
    for it in range(0,niter):
        s= X.dot(Y)
        X,Y=Y,X  
      
def test(p,X,Y,nit):
   
    niter=nit
    T=0.
    while True:
        Init(X,1.)
        Init(Y,1.)
        t1 = time.time()
        p(X,Y,niter)
        treal=time.time() -t1
        t = treal/niter
        if treal>0.0001 and abs(t-T)/t<0.025:
            break
        else:
            T=t
            niter*=2

    return T,niter

size=16
sizemax=10**6
niter=10
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
    flops=nflops/tbest
    print("\nbest: ",parsef(best))
    print("nb. flops (best): ",nflops, ", Gflops/s: ",flops/(10**9))
    print("-------")
    size*=2
    print(" ")
