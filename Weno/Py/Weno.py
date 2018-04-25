import numpy as np
class Weno:
    def __init__(self,size):
        self.c=np.array([
            [11./6.,-7./6.,1./3.],
            [1./3.,5./6.,-1./6.],
            [-1./6.,5./6.,1./3.],
            [1./3.,-7/6.,11./6.]
        ])
        self.dright=np.array([3./10.,3./5.,1./10.])
        self.dleft=np.array([1./10.,3./5.,3.10])
        self.b0=13./12.
        self.b1=1./4.
        self.epsilon=1.e-6
        self.size=size
        self.reconstructed=np.empty(2*size)
        self.numflux=np.empty(size)
        self.beta=np.empty(3)
        self.right=np.zeros(3)
        self.left=np.zeros(3)
        self.alpharight=np.zeros(3)
        self.alphaleft=np.zeros(3)
    def weno(self,NumFl,Fl,L,In,Out):
        F=lambda x,y: NumFl.NumFlux(Fl,x,y)
        size=self.size
        h1= -1./(L/size)
        for vol in range(0,size):
            #reconstructions right & left:
 
            for r in range(0,3):
                cr=self.c[r+1]
                cl=self.c[r]
                self.right[r]=0.0
                self.left[r]=0.0
                for j in range(0,3):
                    self.right[r]+= cr[j]*In[(vol-r+j)%size]
                    self.left[r]+=cl[j]*In[(vol-r+j)%size]

            # regularity coefficients
            self.beta[0]=self.b0*\
                          pow(In[vol]-2.0*In[(vol+1)%size]+
                              In[(vol+2)%size],2)+ \
	                      self.b1*pow(3.*In[vol]-4.*In[(vol+1)%size]+In[(vol+2)%size],2)
            self.beta[1]=self.b0*pow(In[(vol-1)%size]-2.0*In[vol]+In[(vol+1)%size],2)+ \
                          self.b1*pow(In[(vol-1)%size]-In[(vol+1)%size],2)
            self.beta[2]=self.b0*pow(In[(vol-2)%size]-2.0*In[(vol-1)%size]+In[vol],2)+ \
	                  self.b1*pow(In[(vol-2)%size]-4.*In[(vol-1)%size]+3*In[vol],2)
            
            
            for r in range(0,3):
                self.alpharight[r]=self.dright[r]/pow(self.epsilon+self.beta[r],2)
                self.alphaleft[r]=self.dleft[r]/pow(self.epsilon+self.beta[r],2)
            sright=self.alpharight.sum()
            sleft=self.alphaleft.sum()

            recleft=self.alphaleft.dot(self.left)
            recright=self.alpharight.dot(self.right)

        #     # reconstructed values:
            self.reconstructed[2*vol]  = recleft/sleft
            self.reconstructed[2*vol+1]= recright/sright
        #compute the numerical fluxes at boundaries:
        for vol in range(0,size):
            self.numflux[vol]=F(self.reconstructed[2*vol+1],
                                self.reconstructed[2*((vol+1)%size)])
        # #now, return RHS to solver:
        for vol  in range(0,size):
            Out[vol]=h1*(self.numflux[vol]-self.numflux[(vol-1)%size])
