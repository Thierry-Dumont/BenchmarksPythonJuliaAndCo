from rando import *
import numpy as np
from Stiffness import *
from RandomTriangle import *
import time
import socket
import cProfile

ntri=10**7
x=np.empty(3)
y=np.empty(3)
mat=np.empty(21)
S=Stiffness()

print("\nVerify that, on the reference element, we are coherent with sage")
print("(see ../sage/):\n")
x[0]=0.0; x[1]=1.0; x[2]=0.0;
y[0]=0.0; y[1]=0.0; y[2]=1.0;
S.op(x,y,mat)


for i in range(0,6):
    print([mat[i*(i+1)//2+j] for j in range(0,i+1)])
print("\nWe must get the same result if we dilate the triangle:\n")
for i in range(0,3):
    x[i]*=2.
    y[i]*=2
S.op(x,y,mat)
for i in range(0,6):
    print([mat[i*(i+1)//2+j] for j in range(0,i+1)])

print("\nNow, start the benchmark:")
ntri=1000000
print(ntri," triangles.")
R = rando()
t1 = time.time()
for tr in range(0,ntri):
    RandomTriangle(R,x,y)
    S.op(x,y,mat)
t=(time.time()-t1)
print("first phase: ",t," seconds.")
t1 = time.time()
for tr in range(0,ntri):
    RandomTriangle(R,x,y)
tr=(time.time()-t1)
print("second phase: ",tr," seconds.")

t-=tr
print("Total time: ",t," seconds.")
print("Time by triangle:", "{:.5e}".format(t/ntri),"second.")
f=open("RunningOn"+socket.gethostname(),"w")   
f.write(str(t/ntri)+"\n")
f.close()
print("fin")
