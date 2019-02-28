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
    "Fortran",
]
cpp="C++"
#-------------------------------------------
# build a dict  n-> computing time for  C++
C={}
with open("../"+cpp+"/RunningOn"+socket.gethostname(), 'r') as file:
    spRef= file.readline().split()
    s= file.readline()
    C[cpp]=float(s.replace("\n",""))
doNotUse=[]
for n in files:
    filename= "../"+n+"/RunningOn"+socket.gethostname()
    p_file = Path(filename)
    if p_file.is_file():
        with open(filename,"r") as file:
            sp= file.readline().split()
            if sp[0]!=spRef[0] or sp[1]!=spRef[1]:
                print()
                print(n,": You did not perform the same compuation as C++")
                print("C++: ",spRef[0],spRef[1])
                print(n,": ",sp[0],sp[1])
                print("We do not use",n,"in the final comparison.")
                doNotUse.append(n)
            else:
                s=file.readline()
                C[n]=float(s.replace("\n",""))

    else:
        print("\n\nFile "+filename+ " does not exists !")
        print("did you run test in "+n+" ?\n\n")
        
Ts=sorted([(n,C[n]/C[cpp]) for n in C],key=lambda x: x[1])

print("\nComputing time / Computing time in C++:\n")
for s in Ts:
    if s[0]!="C++" and not s[0] in doNotUse:
        print("* ",s[0].ljust(12)," : ",str(s[1])[0:5])
print("\n")
