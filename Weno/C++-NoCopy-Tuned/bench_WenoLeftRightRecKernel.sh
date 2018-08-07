#!/usr/bin/env bash
set -e

if [ "$#" -ge 2 ]
then
    OUTPUT="$1"
else
    OUTPUT="bench.dat"
fi

SIZE=10000000
NRUN=5
FREQ=2200
NTEST=23
TOL=0.01

SOURCES="bench_WenoLeftRightRecKernel_O3.cpp bench_checksum.cpp"
CPP_FLAGS="-std=c++14 -O3 -march=native -mtune=native -DNDEBUG"

COMPILERS=(GNU/4.9 GNU/5 GNU/6 GNU/7 GNU/8 Clang/3.8 Clang/5.0 Clang/6.0 Intel/17.0.2 Intel/18.0.3)

module -q purge
rm -f "$OUTPUT.tmp"

# First column
echo TEST >> "$OUTPUT.tmp"
for test in $(seq 1 $NTEST)
do
    echo $test >> "$OUTPUT.tmp"
done

# Other columns
for comp_mod in ${COMPILERS[@]}
do
    module -q load "$comp_mod"

    comp_name="${comp_mod%%/*}/$($CXX -v 2>&1 | grep -Eo '[[:space:]]version[[:space:]]+[0-9.]+' | grep -o '[0-9.]*')"
    echo $comp_name >> "$OUTPUT.tmp"

    for test in $(seq 1 $NTEST)
    do
        echo "$comp_name #$test"

        "$CXX" $CPP_FLAGS -DTEST=$test $SOURCES -o bench
        perf=$(numactl -C0 -m0 ./bench $SIZE $NRUN $FREQ | tail -1 | cut -d " " -f4)
        LANG=C printf "\t%.2f\n" $perf

        while true
        do
            curr_perf=$(numactl -C0 -m0 ./bench $SIZE $NRUN $FREQ | tail -1 | cut -d " " -f4)
            rel_diff=$(echo "($curr_perf - $perf)/$perf" | bc -l)

            LANG=C printf "\t%.2f (%.2f%%)\n" $curr_perf $(echo "100 * $rel_diff" | bc -l)

            if [ $(echo "$curr_perf < $perf" | bc) -eq 1 ]
            then
                perf=$curr_perf
            fi

            if [ $(echo "-$TOL < $rel_diff && $rel_diff < $TOL" | bc) -eq 1 ]
            then
                break
            fi
        done

        LANG=C printf "\t--> %.1f\n" $perf
        LANG=C printf "%.1f\n" $perf >> "$OUTPUT.tmp"
    done

    module -q unload "$comp_mod"
done

cat "$OUTPUT.tmp" | pr -ts' ' --columns $((${#COMPILERS[@]}+1)) > "$OUTPUT"
rm "$OUTPUT.tmp"
