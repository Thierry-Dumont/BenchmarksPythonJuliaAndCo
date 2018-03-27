from rando import *
import numpy as np
from factorMatrix import *
from RandomFeedMatrix import *
import time
import socket

sizeG=128
D={}

size=sizeG
loop=1

# Checking algo
M = np.empty((128, 128+1))
R = rando()
RandomFeedMatrix(M, R)
print("sum(M) = {}".format(np.sum(M)))
factorMatrix(M)
print("sum(Gauss(M)) = {}".format(np.sum(M)))
print("")

while size>2:
    R=rando()
    M=np.empty((size,size+1))
    t1 = time.time()
    for iter in range(0,loop):
        RandomFeedMatrix(M,R)
        factorMatrix(M)
    t=(time.time()-t1)/loop
    D[size]=t
    size//=2
    loop*=8

print("---")
size= sizeG
loop=1
while size>2:
    R=rando()
    M=np.empty((size,size+1))
    t1 = time.time()
    for iter in range(0,loop):
        RandomFeedMatrix(M,R)
    t=(time.time()-t1)/loop
    D[size] -= t 
    size//=2
    loop*=8

    f=open("RuningOn"+socket.gethostname(),"w")   
    for x in sorted(D.keys()):
        f.write(str(x)+" "+str(D[x])+"\n")
    f.close()
