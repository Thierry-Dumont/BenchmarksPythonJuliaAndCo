First, do:
```
mkdir ./Results (if ./Results does not exists).
./gr.py
```
It will parse your results (in the "RunningOn _your_ _hostname_" files) and populate
Results/ with gnuplot plotable files.

Then:
```
gnuplot
gnuplot> load "gpc"
```
will plot the results. We take C++ computing time as unit.


An example of benchmark is Benchmarks/kepler.pdf which was run on a
4 core i5-4670 CPU @ 3.40GHz.
