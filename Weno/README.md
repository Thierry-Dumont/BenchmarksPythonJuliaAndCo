### Description:

These programs perform a numerical solution of the Burghers equation, using Weno
 finite element spatial discretization and the classical TVD
 Runge-Kutta of order 3.
 
See:
_Shu Chi-Wang,_
_Essentially non-oscillatory and weighted essentially non-oscillatory schemes
for hyperbolic conservation laws_, which can be found [here](https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19980007543.pdf).



We compute in a 1 dimensional periodic domain.

### Motivation:

This is a classical computation in hyperbolic PDEs.

It needs to compute with large stencils, min and max functions. As we
compute in dimension 1, the memory footprint is very low (
everything can be stored in the cache).

One can chose between two problems:

* A simple linear convection by a constant field: du/dt = du/dx.
* The Burghers equation:  du/dt = d/dx f(u)  with f(u)= u^2/2.

And one can choose between two different fluxes:

* The Godunov flux.
* The Lax-Friedrichs flux.

The fluxes are discussed when used together with Weno in the
aforementioned paper of _Shu_, so as with the time discretization by a
Runke-Kutta explicit method of order 3.

We use the Weno combination of 3 points stencils, thus giving a
discretization of order 5 when applied to smooth functions.

### Realisation:

Thierry Dumont   tdumont@math.univ-lyon1.fr

### The directories contain:

- **C++**:      pure C++, sequential.

### Note:

All the codes are self-contained; there is no linear algebra and thus
no need for external libraries.

### Running the benchmarks

cd succesively in C++, Ju, Numba Py, Pythran, PyVec,
 PythranVec; then look at the documentation.

#### Results:

Running the codes  will create a file named:  RunningOn"your
hostname".
