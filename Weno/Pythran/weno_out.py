#pythran export weno_out(
def weno_out(NumF,flux,minflux,maxflux,reconstructed,numflux
             ,InC):
    c=np.array([
        [11./6.,-7./6.,1./3.],
        [1./3.,5./6.,-1./6.],
        [-1./6.,5./6.,1./3.],
        [1./3.,-7/6.,11./6.]])
    dright=np.array([3./10.,3./5.,1./10.])
    dleft=np.array([1./10.,3./5.,3.10])
    beta=np.empty(3)
