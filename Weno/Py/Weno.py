import numpy as np
class Weno:
    def __init__(self,size):
        self.c=np.array([
            [11./6.,-7./6.,1./3.],
            [1./3.,5./6.,-1./6.],
            [-1./6.,5./6.,1./3.],
            [1./3.,-7/6.,11./6.]
        ])
        self.b0=13./12.
        self.b1=1./4.
        self.epsilon=1.e-6
        self.size=size
        # allocate all arrays...
        self.dright=np.array([3./10.,3./5.,1./10.])
        self.dleft=np.array([1./10.,3./5.,3.10])
        self.reconstructed=np.empty(2*size+8)
        self.numflux=np.empty(size+1)
        self.beta=np.empty(3)
        self.right=np.empty(3)
        self.left=np.empty(3)
        self.alpharight=np.empty(3)
        self.alphaleft=np.empty(3)
        self.InC=np.empty(size+4)
    def weno(self,NumFl,Fl,L,In,Out):
        F=lambda x,y: NumFl.NumFlux(Fl,x,y)
        size=self.size
        h1= -1./(L/size)
        # build an extended array with phantom cells to deal with periodicity:
        self.InC[0]=In[size-2]
        self.InC[1]=In[size-1]
        self.InC[2:2+size]=In
        self.InC[size+2]=In[0]
        self.InC[size+3]=In[1]
        for vol in range(2,size+2):
            #reconstructions right & left:
 
            for r in range(0,3):
                cr=self.c[r+1]
                cl=self.c[r]
                self.right[r]=0.0
                self.left[r]=0.0
                for j in range(0,3):
                    self.right[r]+= cr[j]*self.InC[vol-r+j]
                    self.left[r]+=cl[j]*self.InC[vol-r+j]

            # regularity coefficients
            self.beta[0]= self.b0*pow(self.InC[vol]-2.0*self.InC[vol+1]+\
                                      self.InC[vol+2],2)+ self.b1* \
                                      pow(3.*self.InC[vol]-4.*self.InC[vol+1]+\
                                          self.InC[vol+2],2)

            self.beta[1]=self.b0*pow(self.InC[vol-1]-2.0*self.InC[vol]+\
                                     self.InC[vol+1],2)+ self.b1*\
                                     pow(self.InC[vol-1]-self.InC[vol+1],2)
            
            self.beta[2]=self.b0*pow(self.InC[vol-2]-2.0*self.InC[vol-1]+\
                                     self.InC[vol],2)+ self.b1*\
                                     pow(self.InC[vol-2]-4.*self.InC[vol-1]+\
                                         3*self.InC[vol],2)
            
            for r in range(0,3):
                self.alpharight[r]=self.dright[r]/pow(self.epsilon+\
                                                      self.beta[r],2)
                self.alphaleft[r]=self.dleft[r]/pow(self.epsilon+\
                                                    self.beta[r],2)
            sright=self.alpharight.sum()
            sleft=self.alphaleft.sum()

            recleft=self.alphaleft.dot(self.left)
            recright=self.alpharight.dot(self.right)

            # reconstructed values:
            self.reconstructed[2*vol]  = recleft/sleft
            self.reconstructed[2*vol+1]= recright/sright
        self.reconstructed[2*size+4:2*size+8]=self.reconstructed[4:8]
        #compute the numerical fluxes at boundaries:
        for vol in range(1,size+1):
            self.numflux[vol]=F(self.reconstructed[2*vol+3],
                                self.reconstructed[2*(vol+1)+2])
        self.numflux[0]=self.numflux[size]
        #now, return RHS to solver:
        for vol  in range(0,size):
            Out[vol]=h1*(self.numflux[vol+1]-self.numflux[vol])
