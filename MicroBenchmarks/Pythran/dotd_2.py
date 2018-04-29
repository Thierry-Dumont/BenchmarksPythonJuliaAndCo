#pythran export dotd_2(float[:],float[:],int)
def dotd_2(X,Y,niter):
    for it in range(0,niter):
        s= X.dot(Y)
        X,Y=Y,X  
