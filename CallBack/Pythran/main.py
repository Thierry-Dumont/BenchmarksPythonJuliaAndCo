from math import exp
import time
from trapz import *
from f import *
from g import *



n=100000000
t1 = time.time()
sum=trapz(f,0.0,1.0,n)
t=(time.time()-t1)
print(t)
print(sum)
t1 = time.time()
sum=trapz(g,0.0,1.0,n)
t=(time.time()-t1)
print(t)
print(sum)
