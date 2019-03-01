
# Numerical benchmarks for Julia, Python.

We try to avoid trivial and non-sens benchmarks (for people doing
numerics!) like fibonacci, sorting and so on.

We put ourself in the shoes of a typical Matlab or Matlab-like
programmer, writing quite short but numerically intensive
programs. 

 Are Python
and Julia easy to use and efficient? We compare them with a C++
optimized implementation (and sometimes with a Fortran one)..

The benchmark(s):

*  :new: Callback: callbacks of small and not so small functions.

* Gaussian:  Gaussian elimination with partial pivoting.
  
* FeStiff: compute the stiffness matrix, for the Poisson equation,
  discretized with P2 finite elements on triangles.
  
*  Weno: a classical solver for hyperbolic equations, in
  dimension 1, with application to Burghers equation and to Convection. 

* Sparse: building a sparse matrix and doing a sparse matrix x vector product.
  
* MicroBenchmarks: very simple benchmarks to show the importance
  of different programing styles. 


We will add other numerical significative benchmarks in the (near) future.

### Dependencies:

#### What you need to install:


- python3
- pip (pip3)
- g++ (and/or clang++)
- gfortran
- lapack
- openblas
- cmake
- gnuplot

You can install them using your distribution tool (apt...).

- julia

:exclamation: Julia :exclamation: since 2018-10-08 programs  need at least  version Version 1.1 (stable
version in 2018-10); note that all programs needed adaptation when moving to this version, and will not run with former ones.

Note also that the version packaged with  Ubuntu 18-04  is older. Install the stable version from
[here](https://julialang.org). Note also that Julia is evolving, and it is
possible that the codes need some adaptation to run with later
versions of the language.


You also need:

- pythran
- scipy
- Numpy
- numba

to install them,  you can just do:

```
pip install pythran
```
and so on...

You can also install them from [conda](https://conda.io/docs/).

Note that, for Pythran it was necessary to create a .pythranrc file in the home directory, to describe which blas is used:
```
[compiler]
blas=openblas
```
# How to run the Benchmarks ?

-1: __First solution:__ Enter the directories CallBack/, FeStiff/,
Gaussian/, MicroBenchmarks/, Sparse/ or Weno/. Then read the README.md, which
explains how to run the (local) benchmark.

-2: Second solution: In each of these directories, you will find a script: runAllTests.sh . This
script will run the benchmark, for every language (and variant).

-3: Third solution: stay here: The script runAllBenchmarks.sh  will run everything for you. Then enter individial directories, and look at README.md to know how to exploit the results. Note that his will take a __long time__ to run, and potential problems will be difficult to diagnose.

The __first solution is certainly the safest__. 


# Results:

Have a look at the [wiki](https://github.com/Thierry-Dumont/BenchmarksPythonJuliaAndCo/wiki), where you can find results obtained on my personal
computer, as well as some considerations on the different benchmarks
and on optimizations implemented.
