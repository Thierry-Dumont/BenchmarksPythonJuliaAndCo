from math import exp
import time
import socket
from trapz import trapz
from f import f
from g import g
from implicit import implicit



#----------------------main program starts here ------------------
loops=10000
n=1000

fic=open("RunningOn"+socket.gethostname(),"w")

# workaround: PyCapsule' object has no attribute '__name__' (see ../Py/main.py)
name={f:"f",g:"g",implicit:"implicit"}

for F in [f,g,implicit]:
    t1 = time.time()
    for i in range(0,loops):
        sum=trapz(F,0.0,1.0,n)
    t=(time.time()-t1)/loops
    #print(F.__name__," ",t," ",sum)
    #fic.write(F.__name__+": "+str(t)+"\n")
    print(name[F]+": ",t," ",sum)
    fic.write(name[F]+": "+str(t)+"\n")
fic.close()
print("end.")
