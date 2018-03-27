
The programs perform a Gaussian elimination with partial pivoting, except the
C++Lib, PyScipy and JuLib which call the Lapack and blas libraries, to perform
a LU factorization. The cost, in term of operation counts is similar.

The  Gaussian elimination with partial pivoting, even if this is something one
should not code in the real life (except for pedagogical purpose) seems to be
a good prototype of numerical algorithm an user would like to code (as it uses
about n^3 floating point operations and array manipulations).

### Realisation:

Thierry Dumont   tdumont@math.univ-lyon1.fr

Benoît Fabrèges  fabreges@math.univ-lyon1.fr

Roland Denis     denis@math.univ-lyon1.fr

### The directories contain:

C++/      pure C++, sequential.

C++Lib/   C++ and lapack + openblas.

Ju/       pure Julia

JuLib/    Julia +  lapack + openblas.


Py/       pure Python

PyVec/    pure Python, vectorized.

PyScipy/  Python + Scipy.

Pythran/  Pythran.

PythranVec/ Pythran vectorized

Numba/    Python + Numba

PLOT /    graphical material

### Dependencies:

#### What you need to install:

- julia

!ZZZ! Julia: runs with version Version 0.6.2 (stable version in March 2018; note
that the Ubuntu 17-10 version is older). Install the stable version from
https://julialang.org


- python3

- pip

Install them with your distribution tool (apt...).

- pythran

- scipy

- Numpy

- numba

for pythran, scipy, numpy numba you can just do:

pip install pythran  and so on...

You can also install them with conda.


- g++ (or clang++)
- lapack
- openblas
- cmake
- gnuplot

Just install the with apt or the corresponding tool of your distribution.

### Running the benchmarks

cd succesively in C++, Ju, Numba Py, Pythran, PyVec, C++Lib, JuLib,
PyScipy PythranVec.

For Ju* diectories, as well as for Numba/ run  ./script


For C++* directories:

mkdir Build

cd Build

cmake ..

make

./run

It will create a file named RuningOn"your hostname".

### Ploting results:

cd PLOT/

Read README file in PLOT/ .
