#pythran export dotd_1(float[:],float[:],int)
def dotd_1(X,Y,niter):
    size=X.size
    for it in range(0,niter):
        s=0.0
        for i in range(0,size):
            s+=X[i]*Y[i]
        X,Y=Y,X
