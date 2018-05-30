import numpy as np

def NumFlux(F,a,b):
    #if a<=b:
    #  return F.minf(a,b)
    #else:
    #  return F.maxf(b,a);
    return np.where(a <= b, F.minf(a, b), F.maxf(b, a))

