#!/usr/bin/python3
#
#comparison c++ and other computations
#
import socket

def parsit(D,l):
    # extract two numbers from a line.
    ll=l.split(" ")
    if len(ll) == 2:
        D[int(ll[0])]=float(ll[1].replace("\n",""))

        
# directoies to explore ---------   
files=[
    "../Py",
    "../Ju",
    "../Pythran",
    "../JuLib",
    "../PyScipy",
    "../PyVec",
    "../PythranVec",
    "../Numba",
    "../C++Lib",
    "../C++"]
cpp="../C++"


#-------------------------------------------
# build a dict  n-> computing time for  C++
C={}
with open(cpp+"/RuningOn"+socket.gethostname(), 'r') as file:
    for line in file:
        parsit(C,line)

#  build a dict  n-> computing time for all directories if files[]
T={}
for n in files:
    T[n]={}
    filename= n+"/RuningOn"+socket.gethostname()
    with open(filename,"r") as file:
        for line in file:
            parsit(T[n],line)
print("all files parsed.")
# Compute ratio time/(time C++).
for n in files:
    D=T[n]
    for k in D.keys():
        if k in C.keys():
            D[k]/=C[k]
print("ratios computed.")       
# create file for gnuplot.
for n in files:
    D=T[n]
    thefile=n.replace("..","./Results")
    print("-file created: ",thefile)
    with open(thefile, 'w') as file:
        kk=sorted([k for k in D.keys()])
        for k in kk:
            file.write(str(k)+" "+str(D[k])+'\n')
            
print("\n see gpc* files to plot with gnuplot.")
