You need cmake !

### Compilation:
```
mkdir Build
cd Build
cmake ..
make
```
This creates the files: run_cl, run_lapl_1 and run_lapl_2.

### Run the code:

from Build/ directory, type:
```
./run_cl
```
or  run_lapl_1 or  run_lapl_2.

By default, we use g++. To can change the compiler to use, for example
clang++,
replace 
```
cmake ..
```
by

```
CXX=clang++ cmake ..
```
