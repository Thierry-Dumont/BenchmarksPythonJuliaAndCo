import numpy as np

def flux(x):
    return 0.5 * np.square(x)

def minf(a,b):
#    if b<=0:
#      return flux(b)
#    elif a>=0:
#      return flux(a)
#    else:
#      return 0.0
    return np.select([b <= 0, a >= 0], [flux(b), flux(a)])

def maxf(a,b):
    return np.maximum(flux(a),flux(b))
