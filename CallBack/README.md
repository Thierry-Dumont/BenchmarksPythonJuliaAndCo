### Description:

We test some examples of callbacks:

- a very simple function (one line of code).

- a function with a conditional.

- a more computationally expensive function (implicitly defined, we use Newton method).

All these functions are integrated using the trapezoidal rule.

### Motivation:

It is often said that, for "small", non expensive functions, the call back depends on the way the function is passed:

- in C++, classical C like function passing (with pointers) is often described as non efficient; using object functions is supposed to allow inlining and should largely improve performances. For computationally expensive functions, the difference should become neglectable.

 - How does Julia performs ? Python, Pythran, Numba ? 

### Author:

Thierry Dumont   tdumont@math.univ-lyon1.fr

### The directories contain:

- **C++-Pointer**: computation in C++, "C" like method (pointers).

-  **C++-lambda**: computation in C++, passing a lambda function, when it is possible.

-  **C++-Fonctor**:  computation in C++, using object functions.

- **Py**:  Python + numpy computation.

- **Pythran**:  Python + numpy + Pythran  computation.

- **Numba**: Python + numpy + Numba  computation.

- **Ju**: Julia computation.


### Running the benchmarks

cd successively in C++-xxx, Py, Pythran, Numba, Ju; then look at the documentation. 

Once you have run the benchmark in **all** directories, go to Results/
and look at the documentation to know how to exploit the results.


