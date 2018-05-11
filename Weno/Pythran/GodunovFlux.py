#pythran export NumFlux(float(float,float),float(float,float), float, float)
def NumFlux(minF,maxF,a,b):
    if a<=b:
      return minF(a,b)
    else:
      return maxF(b,a);
