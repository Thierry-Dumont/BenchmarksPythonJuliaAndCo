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
     "C++-lambda",  "C++-Pointer",
    "Py",
    "Ju",
    "Numba",
]
cpp="C++-Fonctor"
#-------------------------------------------
# build a dict  n-> computing time for  C++
C={}
with open("../"+cpp+"/RunningOn"+socket.gethostname(), 'r') as file:
    for line in file:
        l=line.replace("\n","").split()
        C[l[0]]=float(l[1])
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



with open("result.md","w") as file:
    file.write("  Type      |     f     |    g     |    implicit    \n")
    file.write("------------------ |-----------|----------|---------------- \n")
    for k in Call.keys():
        s=k+"|"
        for sk in C.keys():
            if sk in Call[k].keys():
                s+=str(Call[k][sk])[0:5]+"        | "
            else:
                s+=" none | "
        print(s)
        file.write(s[0:-2]+"\n")
        file.write("------------------ |-----------|----------|---------------- \n")
