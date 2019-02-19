#!/bin/bash
#
#  This script is supposed to run *all* the test, and then compute
#  the final "report" in Results/
#  Not sure it works everywhere. If it does not, improve it, or enter each
#  directory and look at README.md to know what to do. 
#
for i in C++  C++Lib ; do
    echo "Test: "$i
    echo "--- "
    (cd $i; mkdir -p Build; cd Build; cmake ..; make; ./run)
done
for i in Ju Numba  PythranVec Pythran ; do
    echo  "Test: "$i
    echo "--- "
    (cd $i; ./script)
done  
for i in Py PyVec PyScipy; do
    echo  "Test: "$i
    echo "--- "
    (cd $i; python3 ./main.py)
done

echo " "
eche "Make the report:"
mkdir  -p Results/Results
(cd Results; ./gr.py)
echo " "
echo "Everything went well ? If yes,go to ./Results/. You can visualize the"
echo " results with gnuplot. Look at README.md in ./Results"
echo " "
echo " "
