import numpy as np
#pythran export ind(int,int)
def ind(i,j):
    return i*(i+1)//2+j
#pythran export JinvDetTrans(float[:],float[:],float[:])
def JinvDetTrans(x,y,ji):
    ji[0]=-y[0] + y[2]
    ji[1]= y[0] - y[1]
    ji[2] = x[0] - x[2]
    ji[3]= -x[0] + x[1]
#pythran export StiffOut(float[:],float[:],float[:],float[:],float[:],float[:])
def StiffOut(x,y,m,ji,grads,gq):
    JinvDetTrans(x,y,ji)
    for f in range(0,6):
        for p in range(0,3):
            d=6*f+2*p
            grads[d]  = ji[0]*gq[d]+ji[1]*gq[d+1]
            grads[d+1]= ji[2]*gq[d]+ji[3]*gq[d+1]
        det= -(x[1] - x[2])*y[0] + (x[0] - x[2])*y[1] - (x[0] - x[1])*y[2]
        dv=1.0/(6.0*det)
        for i in range(0,6):
            for j in range(0,6):
                m[ind(i,j)]=dv*sum([grads[6*i+2*k]*
                                         grads[6*j+2*k]+ \
                                         grads[6*i+2*k+1]*
                                         grads[6*j+2*k+1] \
                                         for k in range(0,3)])
