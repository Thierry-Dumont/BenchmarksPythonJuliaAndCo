First, do:
```
mkdir ./Results (if ./Results does not exists).
./gr.py
```
It will parse your results (in the "RunningOn _your_ _hostname_*" files) and populate
Results/ with gnuplot plotable files (6 files).

It also creates gnuplot _scripts_  gpc[2-3]-[b-p]:

* [2-3] stands for the spatial dimension (2 or 3)
* [b-p] stands for the type of computation (b for building the matrix,
  p for the matrix x vector product).

So, gpc-3-b is the time to build the matrix in dimension 3. More
  exactly, it is the ratio of the computing time  for the different
  languages/implementations to the computing time in C++.
  
Then:
```
gnuplot
gnuplot> load "gpc-3-b"
```
will do the job in this case (and so on). Recall that we take C++
computing time as unit. 



