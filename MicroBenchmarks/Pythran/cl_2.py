#pythran export cl_2(float[:],float[:],float[:],float[:],int)
def cl_2(A,B,C,D,niter):
    size=A.size
    for it in range(0,niter):
        for i in range(1,size-1):
            A[i]=1.7*B[i]-0.8*C[i]-0.9*D[i]
        A,D=D,A  
