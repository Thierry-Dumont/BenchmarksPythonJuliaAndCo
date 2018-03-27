from rando import *
import numpy as np
from RandomFeedMatrix import *
import time
import socket
from scipy.linalg import lu

sizeG=2048
D={}

size=sizeG
loop=1
sizemin=4
while size>=sizemin:
    R=rando()
    M=np.empty((size,size))
    t1 = time.time()
    for iter in range(0,loop):
        RandomFeedMatrix(M,R)
        P,L,U=lu(M)
    t=(time.time()-t1)/loop
    print("size:",size," t: ",t)
    D[size]=t
    size//=2
    loop*=4
print("---")
size= sizeG
loop=1
while size>=sizemin:
    R=rando()
    M=np.empty((size,size))
    t1 = time.time()
    for iter in range(0,loop):
        RandomFeedMatrix(M,R)
    t=(time.time()-t1)/loop
    #print("size:",size," t: ",t)
    D[size] -= t 
    size//=2
    loop*=4
 
    f=open("RuningOn"+socket.gethostname(),"w")   
    for x in sorted(D.keys()):
        f.write(str(x)+" "+str(D[x])+"\n")
    f.close()
