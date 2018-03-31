
# Numerical benchmarks for Julia, Python.

Try to avoid trivial and non-sens benchmarks (for people doing numerics!)  like fibonacci, sorting and so on.

* Gaussian:  Gaussian elimination with partial pivoting (see README inside).

We will add other numerical significative benchmarks in the (near) future.

### Dependencies:

#### What you need to install:

- julia

!ZZZ! Julia: runs with version Version 0.6.2 (stable version in March 2018; note
that the Ubuntu 17-10 version is older). Install the stable version from
https:/:/:julialang.org


- python3
- pip

You can install them with your distribution tool (apt...).

- pythran
- scipy
- Numpy
- numba

for pythran, scipy, numpy numba you can just do:

pip install pythran
and so on...

You can also install them with conda.

- g++ (or clang++)
- lapack
- openblas
- cmake
- gnuplot

Just install the with _apt_ or the corresponding tool of your distribution.