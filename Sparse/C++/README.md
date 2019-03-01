
### Building the CSR matrix:

I know two methods to build the matrix:

1- We first build a std::map (i,j) -> value (in PreSparse.hpp, PreLapl.hpp).
Then, building the CSR matrix can be done in one simple pass on the
map.

2- Precompute the size of the arrays, allocate them, fill them and
then build the CSR matrix (exactly what we do in Ju, Py, Numba and Pythran).

Although the first method is very convenient, it is much slower (one
reason could be that we make a lot of memory allocations). So we use the
second one, but you can use the first one (look at main: replace calls to
*do_test_arrays* with calls to *dotest_map*).

We could imagine other methods, like using std::vector's, which grow on
demand, but this will probably also be slower than the second method.

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

