#pythran export lapl2d_2(float[:,:],float[:,:],int)
def lapl2d_2(In,Out,niter):
    size=In.shape[0]
    h2= (1./size)**2
    for it in range(0,niter):
        for i in range(1,size-1):
            for j in range(1,size-1):
                Out[i,j]= h2*(
                    In[i-1,j] + In[i,j-1]-
                    4.0*In[i,j]+
                    In[i+1,j]+In[i,j+1])
        #In,Out=Out,In  
