
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
gnuplot> load "vis_cl"
gnuplot> 
```

This will show the results for the 1d laplacian (as a function of the size).

replace "vis_cl" by ""vis_lapl_1" by ""vis_lapl_2"  for the other results.
