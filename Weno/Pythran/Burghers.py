#pythran export capsule flux(float)
def flux(x):
    return 0.5*x*x
#pythran export capsule minf(float,float)
def minf(a,b):
    if b<=0:
      return flux(b)
    elif a>=0:
      return flux(a)
    else:
      return 0.0
#pythran export capsule maxf(float,float)
def maxf(a,b):
     return max(flux(a),flux(b))
