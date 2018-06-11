import numpy as np
from scipy.sparse import csr_matrix
from build import build2,build3
#
def dotest(dim,size):
    if dim==2:
        A= build2(size)
        lenv=size*size
    else:
        A=build3(size)
        lenv=size**3

    X=np.ones((lenv),dtype=float)
    Y=np.empty((lenv),dtype=float)

    print(A.get_shape())
    print(lenv)
    Y=A.dot(X)
#
    
size=64
dotest(3,size)
dotest(2,size)
        
