
Runing _stream_ with 1 thread, that is to say:
```
export OMP_NUM_THREADS=1
./stream
```

gives, on the _reference_ _machine_:

Function|    Best Rate MB/s | Avg time |    Min time  |   Max time|
--------|-------------------|----------|--------------|-----------|
Copy:|            7808.7 |     0.020542 |    0.020490 |    0.020588|
Scale:|            7578.6 |     0.021212|      0.021112 |     0.021387| 
Add:  |          11048.2|     0.021784|     0.021723 |    0.021899| 
Triad:  |        10360.9  |   0.023216  |   0.023164 |    0.023299|

that is to say about a maximum of 11 GB/s or 1.375 G doubles.

The bandwith of the matrix vector x product is about 16 x n in
dimension 2, and the number of floatting point operations is about 9 x
n, and the the Arithmetic Intensity is about 9/16. The Roofline Model
predicts that the maximum atainable performance will be about
1.375*9/16= 0.77 Gflops/s.
