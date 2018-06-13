import numpy as np
#pythran export build3(int)                
def build3(size):
    nc= 7*(size-2)**3 + 36*(size-2)**2 + 60*(size-2)+ 8*4
    row= np.empty((nc),dtype=int)
    col=np.empty((nc),dtype=int)
    v=np.empty((nc),dtype=float)
    h=1./(size-1)
    h2=h*h
    cd=-6/h2
    hd=1./h2
    size2=size*size
    I=lambda i,j,k: i*size2+j*size+k
    #
    count=0
    for i in range(0,size):
        for j in range(0,size):
            for k in range(0,size):
                l=I(i,j,k)
                row[count]=l
                col[count]=l
                v[count]=cd
                count+=1
                for i1 in [-1,1]:
                    if i+i1>=0 and i+i1<size:
                        row[count]=l
                        col[count]=I(i+i1,j,k)
                        v[count]=hd
                        count+=1                    
                    if j+i1>=0 and j+i1<size:
                        row[count]=l
                        col[count]=I(i,j+i1,k)
                        v[count]=hd
                        count+=1
                    if k+i1>=0 and k+i1<size:
                        row[count]=l
                        col[count]=I(i,j,k+i1)
                        v[count]=hd
                        count+=1
    return nc,row,col,v
