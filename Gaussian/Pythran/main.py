from rando import *
import numpy as np
import factorMatrix 
from RandomFeedMatrix import *
import time
import socket

sizeG=2048
sizelim=4
D={}

size=sizeG
loop=1

# Checking algo
M = np.empty((128, 128+1))
R = rando()
RandomFeedMatrix(M, R)
print("sum(M) = {}".format(np.sum(M)))
factorMatrix.factorMatrix(M)
print("sum(Gauss(M)) = {}".format(np.sum(M)))
print("")

while size>=sizelim:
    R=rando()
    M=np.empty((size,size+1))
    t1 = time.clock()
    for iter in range(0,loop):
        RandomFeedMatrix(M,R)
        factorMatrix.factorMatrix(M)
    t=(time.clock()-t1)/loop
    print("size:",size," t: ",t)
    D[size]=t
    size//=2

    loop*=4

print("---")
size= sizeG
loop=1
while size>=sizelim:
    R=rando()
    M=np.empty((size,size+1))
    t1 = time.clock()
    for iter in range(0,loop):
        RandomFeedMatrix(M,R)
    t=(time.clock()-t1)/loop
    print("size:",size," t: ",t)
    D[size] -= t
    size//=2

    loop*=4
    
    f=open("RuningOn"+socket.gethostname(),"w")   
    for x in sorted(D.keys()):
        f.write(str(x)+" "+str(D[x])+"\n")
    f.close()
