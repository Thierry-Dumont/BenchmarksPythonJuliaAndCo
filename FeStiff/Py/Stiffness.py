import numpy as np
class Stiffness:
    def __init__(self):
        self.ji=np.empty((2,2))
        self.grads=np.empty((18,2))
        self.gq=np.array([[-1., -1.],
                          [ 1.,  1.],
                          [-1., -1.],
                          [ 1.,  0.],
                          [ 1.,  0.],
                          [-1.,  0.],
                          [ 0., -1.],
                          [ 0.,  1.],
                          [ 0.,  1.],
                          [ 0., -2.],
                          [-2., -2.],
                          [ 2.,  0.],
                          [ 0.,  2.],
                          [ 2.,  2.],
                          [ 2.,  0.],
                          [ 0.,  2.],
                          [-2., -2.],
                          [-2.,  0.]])



    def op(self,x,y,m):
        self.ji[0,0]=-y[0] + y[2]
        self.ji[0,1]= y[0] - y[1]
        self.ji[1,0]= x[0] - x[2]
        self.ji[1,1]=-x[0] + x[1]

        c1=self.ji[0,0]
        c2= self.ji[0,1]
        self.grads[:,0] = c1*self.gq[:,0] + c2*self.gq[:,1]
        c1=self.ji[1,0]
        c2= self.ji[1,1]
        self.grads[:,1] = c1*self.gq[:,0] + c2*self.gq[:,1]
        
        det= -(x[1] - x[2])*y[0] + (x[0] - x[2])*y[1] - (x[0] - x[1])*y[2]
        dv=1.0/(6.0*det)
        ii=0
        for i in range(0,6):
            i3=3*i
            for j in range(0,i+1):
                j3=3*j
                m[ii]=dv*(self.grads[i3:i3+3,0].dot(self.grads[j3:j3+3,0])
                          +self.grads[i3:i3+3,1].dot(self.grads[j3:j3+3,1])
                          )
                ii+=1
