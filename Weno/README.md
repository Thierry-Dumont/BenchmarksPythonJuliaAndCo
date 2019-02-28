### Description:

These programs perform a numerical solution of some hyperbolic
 equations in a 1 dimensional periodic domain, using Weno 
 finite volume spatial discretization and the classical TVD
 Runge-Kutta of order 3.
 
See:
_Shu Chi-Wang,_
_Essentially non-oscillatory and weighted essentially non-oscillatory schemes
for hyperbolic conservation laws_, which can be found [here](https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19980007543.pdf).


### Motivation:

This is a classical computation in hyperbolic PDEs.

It needs to compute with large stencils, min and max functions. As we
compute in dimension 1, the memory footprint is very low, but the access patterns to
arrays are not so trivial.

One can chose between two problems:

* A simple linear convection by a constant field: du/dt = du/dx.
* The Burghers equation:  du/dt = d/dx f(u)  with f(u)= (1/2) u^2/2.

And one can choose between two different fluxes:

* The Godunov flux.
* The Lax-Friedrichs flux.

The fluxes are discussed when used together with Weno in the
aforementioned paper of _Shu_, so as with the time discretization by a
Runke-Kutta explicit method of order 3.

We use the Weno combination of 3 points stencils, thus giving a
discretization of order 5 when applied to smooth functions.

### Authors:

- Thierry Dumont   tdumont@math.univ-lyon1.fr
- Roland Denis     denis@math.univ-lyon1.fr
- Benoît Fabrèges  fabreges@math.univ-lyon1.fr

### The directories contain:

- **C++**:      pure C++, sequential.
- **C++-Pointers**  :  other sequential C++ implementation.
- **C++-NoCopy**  :  other sequential C++ implementation.
- **C++-Modulo**  :  other sequential C++ implementation.
- **Py**:       pure Python (with numpy).
- **PyVec**:    vectorized Python version.
- **Ju**:       Ĵulia.
- **Numba**:    Python/Numba version.
- **Fortran**:  Fortran version.

### Note:

* It does not seems easy to implement a Pythran version: defining rhe
  RHS of the Runge-Kutta solver needs to compose 3 levels of functions
  (flux, Numerical flux and Weno) and to pass it to the solver. This
  is something the author was unable to do.

* All the codes are self-contained; there is no linear algebra and thus
no need for external libraries.



### Running the benchmarks

cd succesively in C++, Ju, Numba Py; then look at the documentation.

#### Results:

* Running the codes  will create a file named:  RunningOn"your
hostname".

* The C++ code can produce files to visualize the solution with
gnuplot. _Deactivate this for benchmarking!_ 

##### To exploit the results:

Go to Results/ directory. Then, read the doc.

### Results:
Have a look at the  [this page in the Wiki](https://github.com/Thierry-Dumont/BenchmarksPythonJuliaAndCo/wiki/4-The-Weno-benchmark).
