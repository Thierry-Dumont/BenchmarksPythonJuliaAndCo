You need cmake.

### Compilation:
```
mkdir Build
cd Build
cmake ..
make
```
a file "run" is created

### Run the code:
from Build/ directory, type:
```
export OPENBLAS_NUM_THREADS=1
run
```
(as Julia and Python run openblas with one thread). 

By default, we use g++. Tou can change the compiler to use, for example clang++.


