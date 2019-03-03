### Description:

We build sparse matrices and compute matrix x vector products.

Note that we are interested by:

* time to build the matrices.
* time to compute the matrix  x vector products.

(as a function of the **size** of the problem).

Except in C++, the matrix  x vector products rely on libraries and we
can expect to get about the same computing time in every case. 

The time to build the matrices is certainly  dependent on
optimiations, language and so on.

### Motivation:

The matrices are based on the classical uniform  finite difference
approximation of the Laplacian in 2d (5 points stencil) and 3d (7
points stencil). We compute in a square (2d) or a cube (3d) and the
**size** of the problem is the number of points in every dimension for
the square or for the cube, thus 
giving and order of size^2 or size^3 for the matrices.

Such matrices are representative of what should be used with finite
element or fine volume approximations.



### Author:

Thierry Dumont   tdumont@math.univ-lyon1.fr

### The directories contain:

- **C++**: computation in C++.

- **Py**:  Python + numpy computation.

- **Pythran**:  Python + numpy + Pythran  computation.

- **Numba**: Python + numpy + Numba  computation.

- **Ju**: Julia computation.


### Running the benchmarks:

cd successively in C++, Py, Pythran, Numba, Ju; then look at the documentation.

Once you have run the benchmark in **all** directories, go to Results/
and look at the documentation to know how to exploit the results.


### Note:

In C++, using clang++ (clang version 7.0.0-3), it seems impossible to optimize
with -Ox with x>1 (this is a problem with the Clock). If you can help, you are
welcome! 
