
# Numerical benchmarks for Julia, Python.

We try to avoid trivial and non-sens benchmarks (for people doing
numerics!) like fibonacci, sorting and so on.

We try to put ourself in the shoes of a typical Matlab or Matlab-like
programmer, writing quite short but numerically intensive
programs. 

How to write easily  efficient programs? Are Python
and Julia easy to use and efficient? We compare them with a C++
optimized implementation.

The benchmark(s):

* Gaussian:  Gaussian elimination with partial pivoting (see README inside).

We will add other numerical significative benchmarks in the (near) future.

### Dependencies:

#### What you need to install:


- python3
- pip
- g++ (and/or clang++)
- lapack
- openblas
- cmake
- gnuplot

You can install them using your distribution tool (apt...).

- julia

**Z!** for  Julia: programs  need at least  version Version 0.6.2 (stable
version in March 2018; note 
that the Ubuntu 17-10 version is older). Install the stable version from
[here](https://julialang.org).


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
