import numpy as np

#pythran export StiffOut(float[:],float[:],float[:],float[:,:],float[:,:],float[:,:])
def StiffOut(x,y,m,ji,grads,gq):
        a11=-y[0] + y[2]
        a12= y[0] - y[1]
        a21= x[0] - x[2]
        a22=-x[0] + x[1]

        for i in range(0,18):
                grads[i,0] = a11*gq[i,0] + a12*gq[i,1]
                grads[i,1] = a21*gq[i,0] + a22*gq[i,1]
        # grads[:,0] = a11*gq[:,0] + a12*gq[:,1]
        # grads[:,1] = a21*gq[:,0] + a22*gq[:,1]
        
        det= -(x[1] - x[2])*y[0] + (x[0] - x[2])*y[1] - (x[0] - x[1])*y[2]
        dv=1.0/(6.0*det)
        ii=0
        # in the following lines, if we replace 3*i by i3 => cannot compile
        # same when replacing 3*j by j3
        for i in range(0,6):
            i3=3*i
            for j in range(0,i+1):
                j3=3*j
                m[ii]=dv*(grads[3*i:3*i+3,0].dot(grads[3*j:3*j+3,0])
                      +grads[3*i:3*i+3,1].dot(grads[3*j:3*j+3,1]) )
                ii+=1
