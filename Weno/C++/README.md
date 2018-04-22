### Choose your problem and your numerical flux

You must comment/uncomment the following lines in main.cpp
```
  typedef Burghers Problem;
  //typedef Convection Problem;
```
and
```
  typedef GodunovFlux<Problem> NumFlux;
  //typedef LaxFriedrichsFlux<Problem> NumFlux;
```

### Plotting:

In main.cpp, uncomment the line
```
#define DO_GNUPLOT_FILES
```
then, at run time, the code will produce a file _resultXXX_ every 100 steps, and
a file _gpfile_. To plot the solution over time, you can use gnuplot:
```
>gnuplot
load "gpfile"
```
But **CAVEAT**: **comment** the line for benchmarking!


### Compilation:
```
mkdir Build
cd Build
cmake ..
make
```
a file "run" is created.
To use an other compiler (eg. clang++) do:
```
CXX= clang++ cmake ..
make
```

### Run the code:

from Build/ directory, type:
```
./run
```
