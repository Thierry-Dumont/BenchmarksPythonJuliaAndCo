#!/usr/bin/env bash
set -e

SIZE=10000000
NRUN=5
FREQ=2200
NTEST=23
NLAUCH=2

SOURCES="bench_WenoLeftRightRecKernel_O3.cpp bench_checksum.cpp"
CPP_FLAGS="-std=c++14 -O3 -march=native -mtune=native -DNDEBUG"

COMPILERS=(GNU/4.9 GNU/5 GNU/6 GNU/7 GNU/8 Clang/3.8 Clang/5.0 Clang/6.0 Intel/17.0.2 Intel/18.0.3)

module -q purge

# First column
echo TEST
for test in $(seq 1 $NTEST)
do
    echo $test
done

# Other columns
for comp_mod in ${COMPILERS[@]}
do
    echo $comp_mod
    
    module -q load "$comp_mod"
    
    for test in $(seq 1 $NTEST)
    do
        "$CXX" $CPP_FLAGS -DTEST=$test $SOURCES -o bench
        perf=$(numactl -C0 -m0 ./bench $SIZE $NRUN $FREQ | tail -1 | cut -d " " -f4)
        for i in $(seq 2 $NLAUCH)
        do
            curr_perf=$(numactl -C0 -m0 ./bench $SIZE $NRUN $FREQ | tail -1 | cut -d " " -f4)
            if [ $(echo "$curr_perf < $perf" | bc) -eq 1 ]
            then
                perf=$curr_perf
            fi
        done
        LANG=C printf "%.1f\n" $perf
    done

    module -q unload "$comp_mod"
done

>&2 echo "To transform result into columns: cat <file> | pr -ts' ' --columns $((${#COMPILERS[@]}+1))"
