def NumFlux(F,a,b,alpha=1.):
    return 0.5*(F.flux(a)+F.flux(b) - alpha*(b-a))
