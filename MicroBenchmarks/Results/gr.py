#!/usr/bin/python3
#
#comparison between c++ and other computations
#
import socket
import os

def parsit(D,l):
    # extract two numbers from a line, if this is possible.
    ll=l.split(" ")
    if len(ll) == 2:
        D[int(ll[0])]=float(ll[1].replace("\n",""))

        
# directories to explore ---------   
files=[
    "../Py",
    "../Ju",
    "../Numba",
    "../Pythran"
]
cpp="../C++"

types=["_cl","_lapl_1","_lapl_2"]

def doWhatWeMustDo(Dirs,Type):
    #-------------------------------------------
    # build a dict  n-> computing time for  C++
    C={}
    with open(cpp+"/RunningOn"+socket.gethostname()+Type, 'r') as file:
        for line in file:
            parsit(C,line)

    #  build a dict  n-> computing time for all directories in files[]
    T={}
    for n in Dirs:
        T[n]={}
        filename= n+"/RunningOn"+socket.gethostname()+Type
        with open(filename,"r") as file:
            for line in file:
                parsit(T[n],line)
    #print("all files parsed.")
    # Compute ratio time/(time C++).
    for n in Dirs:
        D=T[n]
        for k in D.keys():
            if k in C.keys():
                D[k]/=C[k]
    #print("ratios computed.")       
    # create file for gnuplot.
    for n in Dirs:
        D=T[n]
        thefile=n.replace("..","./results")+Type
        #print("-file created: ",thefile)
        with open(thefile, 'w') as file:
            kk=sorted([k for k in D.keys()])
            for k in kk:
                file.write(str(k)+" "+str(D[k])+'\n')
#------main:
if not os.path.exists("./results"):
    os.makedirs("./results")
    
for t in types:
    doWhatWeMustDo(files,t)
print("\n see  files in ./results (to plot with gnuplot).")

