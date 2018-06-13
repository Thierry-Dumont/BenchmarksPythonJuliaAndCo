import numpy as np
#pythran export build2(int)
def build2(size):
    #how many coefficients ?
    nc= 5*(size-2)**2+ 16*(size-2)+ 12
    row= np.empty((nc),dtype=int)
    col=np.empty((nc),dtype=int)
    v=np.empty((nc),dtype=float)
    h=1./(size-1)
    h2=h*h
    cd=-4/h2
    hd=1./h2;
    I=lambda i,j: i*size+j
    count=0

    for i in range(0,size):
        for j in range(0,size):
             l=I(i,j)
             row[count]=l
             col[count]=l
             v[count]=cd
             count+=1
             for i1 in [-1,1]:
                 if i+i1>=0 and i+i1<size:
                     row[count]=l
                     col[count]=I(i+i1,j)
                     v[count]=hd
                     count+=1
                 if j+i1>=0 and j+i1<size:
                    row[count]=l
                    col[count]=I(i,j+i1)
                    v[count]=hd
                    count+=1
    
    return nc,row,col,v



