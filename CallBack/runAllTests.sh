#!/bin/bash
#
#  This script is supposed to run *all* the test, and then compute
#  the final "report" in Results/
#  Not sure it works everywhere. If it does not, improve it, or enter each
#  directory and look at README.md to know what to do. 
#
for i in C++-Fonctor  C++-lambda  C++-Pointer; do
    echo "--- "
    echo "Test: "$i
    echo "--- "
    (cd $i; mkdir -p Build; cd Build; cmake ..; make; ./run)
done
for i in Ju Numba Pythran ; do 
     echo "--- " 
    echo  "Test: "$i
    echo "--- "
    (cd $i; ./script)
done

echo "--- "
echo  "Test: Py"
echo "--- "
(cd Py; python3 ./main.py)


echo " "
echo "Make the report:"
(cd Results; ./Look.py)
echo " "
echo "To replay the report, cd Results/ and run ./Look.py "
echo " "
