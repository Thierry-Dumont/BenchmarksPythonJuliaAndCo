#pythran export lapl1d_1(float[:],float[:],int)
def lapl1d_1(In,Out,niter):
    size=In.size
    h2= (1./size)**2
    for it in range(0,niter):
        Out[1:size-1]= h2*(In[0:size-2]- 2.0*In[1:size-1]+ In[2:size])
        In,Out=Out,In
