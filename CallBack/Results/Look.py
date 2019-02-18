#!/usr/bin/python3
#
#comparison between a reference computaion and other computations
#
import socket

# directories to explore ---------   
files=[
     "C++-lambda",  "C++-Pointer",
    "Py",
    "Pythran",
    "Numba",
    "Ju",
    
]
#  ----  Directory of the reference program (C++?), from which computing time
# will be taken as  units
# --------------------------------------------------------------------------
cpp="C++-Fonctor"

#---------------------------------------------------------------------------
# build a dict  n-> computing time for "cpp" directory
C={}
with open("../"+cpp+"/RunningOn"+socket.gethostname(), 'r') as file:
    for line in file:
        l=line.replace("\n","").split()
        C[l[0]]=float(l[1])
        
#---- now loop on files [] and compare, if possible.
Call={}
for n in files:
    Cloc={}
    filename= "../"+n+"/RunningOn"+socket.gethostname()
    if n == "Ju":
        with open(filename,"r") as file:
            for line in file:
                l=line.replace("\n","").split()
                if len(l)!=0:
                    s=l[1].replace("Trial(","").replace(" μs","")
                    if l[0] in C.keys():
                        Cloc[l[0]]=float(s)*1.e-6/C[l[0]]         
                    
    else:
        with open(filename,"r") as file:
            for line in file:
                l=line.replace("\n","").split()
                if l[0] in C.keys():
                    Cloc[l[0]]=float(l[1])/C[l[0]]
    Call[n]=Cloc

#----- print and save results:

with open("result.md","w") as file:
    print("\n"+" ".join([l.ljust(15) for l in ["Type","  f","  g","implicit"]]))
    file.write("  Type      |     f     |    g     |    implicit    \n")
    file.write("------------------ |-----------|----------|---------------- \n")
    for k in Call.keys():
        s=k+"|"
        for sk in C.keys():
            if sk in Call[k].keys():
                s+=str(Call[k][sk])[0:5]+"        | "
            else:
                s+=" none | "

        print(" ".join([str(l).ljust(15) for l in s.split("|")]))
        file.write(s[0:-2]+"\n")
        file.write("------------------ |-----------|----------|---------------- \n")
