
### Compilation:

(You need cmake).
```
mkdir Build
cd Build
cmake ..
make
```

a file "run" is created.

### Run the code:

from Build/ directory, type:
```
./run
```
By default, we use g++. To can change the compiler use, for example to
use clang++, replace:
```
cmake ..
```
by:
```
CXX=clang++ cmake ..
```
