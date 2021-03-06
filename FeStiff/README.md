### Description:

These programs compute the elementary stiffness matrix for the Poisson
equation on a collection of  randomly chosen triangles, using 
2-dimensional  P2 Lagrange finite elements (every book on finite elements
describes this).


### Motivation:

This computation is simple, but representative of many numerical
computations. It is auto-contained;  there is no linear algebra and thus
no need for external libraries.

### Author:

Thierry Dumont   tdumont@math.univ-lyon1.fr

### The directories contain:

- **C++**: computation in C++.

- **Py**:  Python + numpy computation.

- **Pythran**:  Python + numpy + Pythran  computation.

- **Numba**: Python + numpy + Numba  computation.

- **Ju**: Julia computation.

- **sage**: _SageMath_ material (see below).
### Running the benchmarks

cd successively in C++, Py, Pythran, Numba, Ju; then look at the documentation.

Once you have run the benchmark in **all** directories (except sage,
which is not for benchmarking!), go to Results/
and look at the documentation to know how to exploit the results.

#### Sage material:

In directory "sage": how to use a (free!) computer algebra system to
 help compute and implement finite elements. 

### Results:
Have a look at the  [this page in the Wiki](https://github.com/Thierry-Dumont/BenchmarksPythonJuliaAndCo/wiki/5-The-FeStiff-benchmark).
