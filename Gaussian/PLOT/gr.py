#!/usr/bin/python3
#comparaison c++ et autres calculs
import socket

def parsit(D,l):
    # decompose une ligne en 2 nombres.
    ll=l.split(" ")
    if len(ll) == 2:
        D[int(ll[0])]=float(ll[1].replace("\n",""))

        
#les repertoires à explorer ---------   
files=[
    "../Py",
    "../Ju",
    "../Pythran",
    "../JuLib",
    "../PyScipy",
    "../PyVec",
    "../PythranVec",
    "../Numba",
    "../C++Lib","../C++"]
cpp="../C++"


#-----------------------------------
#construire un dictionnaire n-> durée pour C++
C={}
with open(cpp+"/RuningOn"+socket.gethostname(), 'r') as file:
    for line in file:
        parsit(C,line)

#construire un dictionnaire n-> durée pour chaque répertoire de "files"
T={}
for n in files:
    T[n]={}
    filename= n+"/RuningOn"+socket.gethostname()
    with open(filename,"r") as file:
        for line in file:
            parsit(T[n],line)
print("all files parsed.")
# Calculer le ratio temps/ temps C++
for n in files:
    D=T[n]
    for k in D.keys():
        if k in C.keys():
            D[k]/=C[k]
print("ratios computed.")       
# créer les fichiers (pour gnuplot).
for n in files:
    D=T[n]
    thefile=n.replace("..","./Results")
    print("-file created: ",thefile)
    with open(thefile, 'w') as file:
        kk=sorted([k for k in D.keys()])
        for k in kk:
            file.write(str(k)+" "+str(D[k])+'\n')
            
print("\n see gpc* files to plot with gnuplot.")
