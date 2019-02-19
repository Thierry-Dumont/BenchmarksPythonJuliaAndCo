#!/bin/bash
#
#  This script is supposed to run *all* the test, and then compute
#  the final "report" in Results/
#  Not sure it works everywhere. If it does not, improve it, or enter each
#  directory and look at README.md to know what to do. 
#

echo "--- "
echo "Test: C++"
echo "--- "
(cd C++; mkdir -p Build; cd Build; cmake ..; make; ./run)

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
eche "Make the report:"
mkdir  -p Results/Results
(cd Results; ./gr.py)
echo " "
echo "Everything went well ? If yes,go to ./Results/. You can visualize the"
echo " results with gnuplot. Look at README.md in ./Results"
echo " "
echo " "
