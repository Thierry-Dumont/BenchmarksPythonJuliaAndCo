from numba import jit
@jit
def RandomTriangle(R,x,y):
    # domain is [0,10]x[0,10]
    for i in range(0,3):
        x[i]=R.fv(10.)
    for i in range(0,3):
        y[i]=R.fv(10.)
