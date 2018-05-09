import numpy as np
from numba import jitclass,float64,int64
spec=[
    ('ji',float64[:]),
    ('grads',float64[:]),
    ('gq',float64[:]),
    ]
@jitclass(spec)
class Stiffness:
    def __init__(self):
        self.ji=np.empty(4)
        self.grads=np.empty(36)
        self.gq=np.array([-1., -1., 1., 1., -1., -1., 1., 0., 1., 0.,
                          -1., 0., 0., -1., 0., 1., 0., 1., 0., -2., -2.,
                          -2., 2., 0., 0., 2., 2., 2., 2.,0., 0., 2., -2.,
                          -2., -2., 0.])
    def ind(self,i,j):
        return i*(i+1)//2+j
    def JinvDetTrans(self,x,y):
        self.ji[0]=-y[0] + y[2]
        self.ji[1]= y[0] - y[1]
        self.ji[2] = x[0] - x[2]
        self.ji[3]= -x[0] + x[1]
    def op(self,x,y,m):
        self.JinvDetTrans(x,y)
        for f in range(0,6):
            for p in range(0,3):
                d=6*f+2*p
                self.grads[d]  = self.ji[0]*self.gq[d]+self.ji[1]*self.gq[d+1]
                self.grads[d+1]= self.ji[2]*self.gq[d]+self.ji[3]*self.gq[d+1]
        det= -(x[1] - x[2])*y[0] + (x[0] - x[2])*y[1] - (x[0] - x[1])*y[2]
        dv=1.0/(6.0*det)
        for i in range(0,6):
            for j in range(0,6):
                s=0.0
                for k in range(0,3):
                    s+=self.grads[6*i+2*k]*self.grads[6*j+2*k]+ \
                        self.grads[6*i+2*k+1]*self.grads[6*j+2*k+1]
                m[self.ind(i,j)]=s*dv
                # this does not compile:
                # m[self.ind(i,j)]=dv*sum([self.grads[6*i+2*k]*
                #                          self.grads[6*j+2*k]+ \
                #                          self.grads[6*i+2*k+1]*
                #                          self.grads[6*j+2*k+1] \
                #                          for k in range(0,3)])
