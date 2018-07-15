#!/usr/bin/python3
#
#comparison between c++ and other computations
#
import socket
def parsit(D,name,l):
    #
    D[name]=float(l.replace("\n",""))
# directories to explore ---------   
files=[
    "Py",
    "PyVec",
    "Ju",
    "Numba",
    "Fortran",
    "C++-Modulo",
]
cpp="C++"
#-------------------------------------------
# build a dict  n-> computing time for  C++
C={}
with open("../"+cpp+"/RunningOn"+socket.gethostname(), 'r') as file:
    for line in file:
        C[cpp]=float(line.replace("\n",""))
for n in files:
    filename= "../"+n+"/RunningOn"+socket.gethostname()
    with open(filename,"r") as file:
        for line in file:
            C[n]=float(line.replace("\n",""))


Ts=sorted([(n,C[n]/C[cpp]) for n in files],key=lambda x: x[1])

print("\nComputing time / Computing time in C++:\n")
for s in Ts:
    print("* ",s[0].ljust(9)," : ",str(s[1])[0:5])
print("\n")
