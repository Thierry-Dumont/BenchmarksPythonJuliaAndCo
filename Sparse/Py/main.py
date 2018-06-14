import numpy as np
import time
import socket
from scipy.sparse import csr_matrix
from build import build2,build3
#
def dotest(dim,size):
    tb = time.time()
    if dim==2:
        A,nc= build2(size)
        lenv=size*size
    else:
        A,nc=build3(size)
        lenv=size**3
    tb=time.time()-tb
    X=np.ones((lenv),dtype=float)
    Y=np.empty((lenv),dtype=float)

    

    tprod=time.time()
    Y=A.dot(X)
    tprod=time.time()-tprod

    return lenv,nc,tb,tprod
#
def banner(dim):
    print("\n",str(dim)+"d")
    print("size".ljust(5),"order".ljust(8),"nc".ljust(8),
          "T.b".ljust(21),"T.p".ljust(20))
    

# lets's go:     
for dim in [2,3]:
    if dim==2:
        size=32
        sizeM=2048
    else:
        size=16
        sizeM=128
    banner(dim)
    f=open("RunningOn"+socket.gethostname()+"-"+str(dim),"w")
    while size<=sizeM:
        order,nc,tb,tm=dotest(dim,size)
        tw=str(size).ljust(6)+str(order).ljust(10)+str(nc).ljust(10)+\
            str(tb).ljust(22)+str(tm).ljust(20)
        print(tw)
        f.write(tw+"\n")
        size*=2
    f.close()  
