
### Building the CSR matrix:

We first build a std::map (i,j) -> value (in PreSparse.hpp, PreLapl.hpp).

Then, building the CSR matrix can be done in one simple pass on the
map (in Csr.hpp).

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
./run
```

