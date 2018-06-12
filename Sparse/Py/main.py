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
    print("size".ljust(4),"order".ljust(8),"nc".ljust(10),
          "T.b".ljust(22),"T.p".ljust(20))
    
size=32
sizeM=2048
banner(2)
while size<=sizeM:
    order,nc,tb,tm=dotest(2,size)
    print(str(size).ljust(4),str(order).ljust(8),str(nc).ljust(10),
          str(tb).ljust(22),str(tm).ljust(20))
    size*=2
size=16
sizeM=128
banner(3)
while size<=sizeM:
    order,nc,tb,tm=dotest(3,size)
    print(str(size).ljust(4),str(order).ljust(8),str(nc).ljust(10),
          str(tb).ljust(22),str(tm).ljust(20))
    size*=2

        
