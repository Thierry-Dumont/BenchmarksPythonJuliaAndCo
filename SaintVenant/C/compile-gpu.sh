#!/bin/bash

#module load GNU/7.1
#module load Eigen
nvcc -std=c++14 -arch=sm_35 -O3 -DNDEBUG -g -lineinfo main1d-gpu.cu main1d-gpu-kernels.cu -o main1d-gpu --expt-relaxed-constexpr
