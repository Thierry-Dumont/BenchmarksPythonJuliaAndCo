#!/bin/bash

#module load GNU/7.1
#module load Eigen
g++ -std=c++14 -march=native -O3 -DNDEBUG main1d.cpp -o main1d
