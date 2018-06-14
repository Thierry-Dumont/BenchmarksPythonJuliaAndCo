#!/usr/bin/python3
#
#comparison between c++ and other computations
#
import socket

def parsit(D,l):
    # extract two numbers from a line, if this is possible.
    ll=[s for s in l.split(" ") if s !=""]
    D[int(ll[2])]=[float(ll[3].replace("\n","")),
                   float(ll[4].replace("\n",""))]

        
# directories to explore ---------   
files=[
    "../Py",
    "../Ju",
    "../Pythran",
    "../Numba",
    ]
cpp="../C++"

for dim in ["-2","-3"]:
    #-------------------------------------------
    # build a dict  n-> computing time for  C++
    C={}
    with open(cpp+"/RunningOn"+socket.gethostname()+dim, 'r') as file:
        for line in file:
            parsit(C,line)
            #  build a dict  n-> (building,computing) time for
            #all directories in files[]
    T={}
    for n in files:
        T[n]={}
        filename= n+"/RunningOn"+socket.gethostname()+dim
        with open(filename,"r") as file:
            for line in file:
                parsit(T[n],line)
    print(dim,": all files parsed.")
    # Compute ratio time/(time C++).
    for n in files:
        D=T[n]
        for k in D.keys():
            if k in C.keys():
                D[k][0]/=C[k][0]
                D[k][1]/=C[k][1]
    print("ratios computed.")       
    # create file for gnuplot.
    for n in files:
        D=T[n]
        thefile=n.replace("..","./Results")+dim
        print("-file created: ",thefile)
        with open(thefile, 'w') as file:
            kk=sorted([k for k in D.keys()])
            for k in kk:
                file.write(str(k)+" "+str(D[k][0])+" "+str(D[k][1])+'\n')

    for typv in ["-b","-p"]:            
        Outf="gpc"+dim+typv
        
        f=open(Outf,"w")
        f.write("set logscale\n")
        f.write("set key top right\n")
        f.write('set xlabel "Number of unknowns"\n')
        f.write('set ylabel "cpu time / cpu time C++"\n')

        first="plot "
        if typv=="-b":
            f.write('set title "Building the matrix"\n') 
        else:
            f.write('set title "Matrix x Vector product"\n') 
        for n in files:
            fil="./Results/"+n.replace("../","")+dim
            if typv=="b":
                fieldv="2"
            else:
                fieldv="3"
                title=n.replace("../","'")+"'"
            s=first+"'"+fil+"' using 1:"+fieldv+" with linespoints title "+\
               title+" \n"
            f.write(s)
            first="replot "
        f.close()
        print("Created ./"+Outf)
print("\nIn gnuplot, load one of the  gpc* files, for example, do:")
print('gnuplot> load "gpc-2-b"')
