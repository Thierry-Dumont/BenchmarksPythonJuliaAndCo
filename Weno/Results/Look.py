#!/usr/bin/python3
#
#comparison between c++ and other computations
#
import socket
from pathlib import Path

def parsit(D,name,l):
    #
    D[name]=float(l.replace("\n",""))
# directories to explore ---------   
files=[
    "C++NoCopy",
    "Py",
    "PyVec",
    "Ju",
    "Numba",
    "Fortran",
    "C++-Modulo",
    "C++-Pointers",
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
    p_file = Path(filename)
    if p_file.is_file():
        with open(filename,"r") as file:
            for line in file:
                C[n]=float(line.replace("\n",""))

    else:
        print("\n\nFile "+filename+ " does not exists !")
        print("did you run test in "+n+" ?\n\n")
        
Ts=sorted([(n,C[n]/C[cpp]) for n in C],key=lambda x: x[1])

print("\nComputing time / Computing time in C++:\n")
for s in Ts:
    if s[0]!="C++":
        print("* ",s[0].ljust(12)," : ",str(s[1])[0:5])
print("\n")
