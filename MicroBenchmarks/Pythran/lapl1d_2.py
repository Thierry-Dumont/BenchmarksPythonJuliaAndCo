#pythran export lapl1d_2(float[:],float[:],int)
def lapl1d_2(In,Out,niter):
    size=In.size
    h2= (1./size)**2
    for it in range(0,niter):
        for i in range(1,size-1):
            Out[i]=h2*(In[i-1]- 2.0*In[i]+ In[i+1])
        In,Out=Out,In  
