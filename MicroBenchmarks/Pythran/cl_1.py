#pythran export cl_1(float[:],float[:],float[:],float[:],int)
def cl_1(A,B,C,D,niter):
    size=A.size
    for it in range(0,niter):
        A=1.7*B-0.8*C-0.9*D
        A,D=D,A
