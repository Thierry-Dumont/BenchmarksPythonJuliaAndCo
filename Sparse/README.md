### Description:

These programs build a sparse matrix (CSR) and perform a matrix-vector
product.

We build a matrix based on the classical finite differences Laplacian
operator on uniform cartesian grids (5 points stencil in dimension 2,
7 points in dimension 3). 

### Motivation:

This is simple; this is also something one would never do but which is
representative of the discretisation of an elliptic PDE. 

Python (with scipy/numpy) and Julia propose objects and routines to
build and compute with these objects.  For C++, we propose our own
implementation. 

### Author:

Thierry Dumont   tdumont@math.univ-lyon1.fr

### The directories contain:

- **C++**:      pure C++, sequential.
- **Py**:       pure Python.
- **Ju**:       Ĵulia.
- **Numba**:    Python/Numba version.


### Note:

* All the computations are sequential.



### Running the benchmarks

cd succesively in C++, Ju, Numba Py; then look at the documentation.

#### Results:

* Running the codes  will create a file named:  RunningOn"your
hostname".



##### To exploit the results:

Go to Results/ directory. Then, read the doc.
