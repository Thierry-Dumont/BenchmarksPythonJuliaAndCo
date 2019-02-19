#!/bin/bash

for d  in CallBack  Gaussian  FeStiff MicroBenchmarks  Sparse  Weno; do
    zcho " "
    echo "===================================================="
    echo "         Benchmark in " $d
    echo "===================================================="
    echo " "
    echo " "
    (cd $d; ./runAllTests.sh)
done
