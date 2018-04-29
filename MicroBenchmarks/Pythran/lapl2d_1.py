#pythran export lapl2d_1(float[,],float[,],int)
def lapl2d_1(In,Out,niter):
    siz=In.shape[0]

    h2= (1./siz)**2

    for it in range(0,niter):
        Out[1:siz-1,1:siz-1]= h2*(
            In[0:siz-2,1:siz-1 ] + In[1:siz-1,0:siz-2]-
            4.0*In[1:siz-1,1:siz-1]+
            In[2:siz,1:siz-1]+In[1:siz-1,2:siz])
        #In,Out=Out,In
