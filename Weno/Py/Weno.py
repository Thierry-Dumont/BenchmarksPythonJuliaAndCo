import numpy as np
c=np.array([
    [11./6.,-7./6.,1./3.],
    [1./3.,5./6.,-1./6.],
    [-1./6.,5./6.,1./3.],
    [1./3.,-7/6.,11./6.]
  ])
dright=np.array([3./10.,3./5.,1./10.])
dleft=np.array([1./10.,3./5.,3.10])
b0=13./12.
b1=1./4.
epsilon=1.e-6

def Weno(NumFl,Fl,L,In,Out):
    F=lambda x,y: NumFl.NumFlux(Fl,x,y)
    size=In.shape[0]
    h1= -1./(L/size)
    reconstructed=np.empty(2*size)
    numflux=np.empty(size)
    beta=np.empty(3)
    for vol in range(0,size):
        #reconstructions right & left:
        right=np.zeros(3)
        left=np.zeros(3)
        for r in range(0,3):
            cr=c[r+1]
            cl=c[r]
            for j in range(0,3):
                right[r]+= cr[j]*In[(vol-r+j)%size]
                left[r]+=cl[j]*In[(vol-r+j)%size]
        #print("left",vol,left)       
        #print("right",right)
           
        # regularity coefficients
        beta[0]=b0* pow(In[vol]-2.0*In[(vol+1)%size]+In[(vol+2)%size],2)+ \
	         b1*pow(3.*In[vol]-4.*In[(vol+1)%size]+In[(vol+2)%size],2)
        beta[1]=b0*pow(In[(vol-1)%size]-2.0*In[vol]+In[(vol+1)%size],2)+ \
                 b1*pow(In[(vol-1)%size]-In[(vol+1)%size],2)
        beta[2]=b0*pow(In[(vol-2)%size]-2.0*In[(vol-1)%size]+In[vol],2)+ \
	         b1*pow(In[(vol-2)%size]-4.*In[(vol-1)%size]+3*In[vol],2)
        #print("beta",vol,beta);
        alpharight=np.zeros(3)
        alphaleft=np.zeros(3)
        for r in range(0,3):
            alpharight[r]=dright[r]/pow(epsilon+beta[r],2)
            alphaleft[r]=dleft[r]/pow(epsilon+beta[r],2)
        #print("alphaleft",vol,alphaleft)
        #print("alpharight",vol,alpharight)
        sright=alpharight.sum()
        sleft=alphaleft.sum()
        #print("s",vol,sleft,sright,"\n")
        recleft=alphaleft.dot(left)
        recright=alpharight.dot(right)
        #print("rec",vol,recleft,recright,"\n")
        # reconstructed values:
        reconstructed[2*vol]  = recleft/sleft
        reconstructed[2*vol+1]= recright/sright
    #print("reconstructed",reconstructed)
    #compute the numerical fluxes at boundaries:
    for vol in range(0,size):
        numflux[vol]=F(reconstructed[2*vol+1], reconstructed[2*((vol+1)%size)])
    #print("numflux",numflux)
    #print("h1",h1)
    #now, return RHS to solver:
    #Out[0]= h1*(numflux[0]-numflux[size-1]);
    for vol  in range(0,size):
      Out[vol]=h1*(numflux[vol]-numflux[(vol-1)%size])
    #print("Out",Out)
