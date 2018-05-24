
Just do:

```
./gr.py
```
This will create a directory "results/" if it does not exists, and
populate it with  files named:


AAA_BBB with AAA= Ju, Py, Numba or Pythran and BBB= cl, lapl_1 or lapl_2

These file contain the CPU time divided by the  CPU time
for the corresponding C++ program (that is to say, we take the CPU time
in C++ as unit).

For visualisation you can do for example:
```
gnuplot
gnuplot> set logscale
gnuplot> plot "Ju_lapl_1","Numba_lapl_1","Py_lapl_1","Pythran_lapl_1"
```

This will show the results for the 1d laplacian (as a function of the size).

replace "lapl_1" by "lapl_2" or "_cl" for the other computations.
