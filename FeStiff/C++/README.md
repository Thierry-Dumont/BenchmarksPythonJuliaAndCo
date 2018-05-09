You need cmake

Compilation:
-----------

```
mkdir Build
cd Build
cmake ..
make
```
a file "run" is created

Run the code:
------------
from Build/ directory, type:
```
run
```

By default, we use g++. You can change the compiler to use, for
example clang++.
For this just replace:

```
cmake ..
```
by:
```
CXX=clang++ cmake ..
```
