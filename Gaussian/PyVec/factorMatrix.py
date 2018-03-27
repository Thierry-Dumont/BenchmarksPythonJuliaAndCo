import numpy as np
def factorMatrix(M):
# Gaussian elimination, partial pivoting.
# M must be an (n,n+1) numpy array. Not tested!
    n = M.shape[0]
    m=  M.shape[1]
    for line in range(0, n-1):
        # find pivot
        cmax = np.argmax(abs(M[line:n,line])) + line

        # exchange rows if necessary
        if cmax != line:
            M[[line,cmax]]=M[[cmax,line]]

        # eliminate
        pivot = M[line,line]
        v = M[(line+1):n,[line]]/pivot
        M[(line+1):n,line:m] -= v*M[[line],line:m]
