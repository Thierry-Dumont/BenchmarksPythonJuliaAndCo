#!/bin/bash
#
#  This script is supposed to run *all* the test, and then compute
#  the final "report" in Results/
#  Not sure it works everywhere. If it does not, improve it, or enter each
#  directory and look at README.md to know what to do. 
#
echo "Test: C++"
echo "--- "
(cd C++; mkdir -p Build;cd Build; cmake ..; make; ./run_cl;./run_lapl_1;./run_lapl_2)

for i in Ju Numba ; do
    echo  "Test: "$i
    echo "--- "
    (cd $i; for j in main*; do ./script $j; done)
done
echo  "Test: Py"
echo "--- "
(cd Py; python3 main_cl.py; python3 main_lapl_1d.py; python3 main_lapl_2d.py)
     
echo  "Test: Pythran"
echo "--- "
(cd Pythran; ./script)
echo " "

echo "Make the report:"
mkdir  -p Results/results
(cd Results; ./gr.py)
echo " "
echo "Everything went well ? If yes, go to ./Results/. You can visualize the"
echo " results with gnuplot. Look at README.md in ./Results"
echo " "
echo " "
