import numpy as np
import time
import socket
from scipy.sparse import csr_matrix
import build2
import build3
#
def dotest(dim,size):
    tb = time.time()
    if dim==2:
        nc,row,col,v= build2.build2(size)
        lenv=size*size
    else:
        nc,row,col,v= build3.build3(size)
        lenv=size**3
    A=csr_matrix((v, (row, col)))
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
    print("size".ljust(5),"order".ljust(9),"nc".ljust(9),
          "T.b".ljust(21),"T.p".ljust(20))
        
for dim in [2,3]:
    if dim==2:
        size=32
        sizeM=2048
    else:
        size=16
        sizeM=256
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
