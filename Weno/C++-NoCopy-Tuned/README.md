# Differences with the C++-NoCopy version

The differences between this version and the `C++-NoCopy` version are:
- storing results of each branch of the ternary operator in `GodunovFlux.hpp`
- storing Weno reconstructions in a temporary vector before calculating the
  finite differences in `Weno.hpp` (this version is strictly not a "NoCopy"
  version)

The preprocessor constant `USE_ORIGINAL_VERSION` can be set in order to disable
these modifications.

Using 1000 discretization points, this version runs the general Weno benchmark
in 0.0945s (using GNU 8.1.0 on an Intel Xeon E5-2650 v4 @ 2.20GHz) compared
with 0.147s for the `C++-NoCopy` version, and 0.100s for the `Fortran` version.

Using 10000 discretization points, this version runs in 6.45s, the `C++-NoCopy`
version runs in 10.97s and the `Fortran` version runs in 7.30s.


# Why this "tuned" version ?

While investigating the performances difference of Fortran and C++
(especially the `C++-NoCopy` version) on the Weno test, it arises that
the C++ binaries lack of vectorization.
More precisely, the tested Fortran compilers are able to mainly use AVX 256bits
instructions for the Weno reconstruction kernel while the C++ compilers
were almost always limited to AVX 128bits.

## The Weno kernel benchmark
The file `bench_WenoLeftRightRecKernel_O3.cpp` contains some experiments
on the compiler's skills (in `-03` optimization mode) to generate fast binary
of the `WenoLeftRightRecKernel` function of `Weno.hpp` depending on the way
its result is manipulated.

It seems that the operations applied to the kernel results affect the compiler
ability to generate fast binary of the same kernel with performance penalty
up to a factor 3.
Even equivalent ways of doing the same operations affect the performances.

### How to run this micro benchmark
This benchmark can be compiled with
```bash
$C++ -std=c++14 -03 -march=native -mtune=native -DNDEBUG -DTEST=N bench_WenoLeftRightRecKernel_O3.cpp bench_checksup.cpp -o bench
```
where `N` is a integer between 1 and 23 that specifies which operations
should be applied to the Weno kernel results
(see `bench_WenoLeftRightRecKernel_O3.cpp`).

The binary can be run with
```bash
numactl -C0 -m0 ./bench SIZE NRUN FREQ
```
where SIZE is the size of the discretized space on which the kernel is applied
(typically 10000000), NRUN is the number of consecutive runs (5) and FREQ
is your processor frequency (2200) in order to accuratly calculate the number
of cycles per execution of the kernel.

In order to have accurate timings, you should disable some energy saving
features like TurboBoost (see https://askubuntu.com/a/619881).

The `bench_WenoLeftRightRecKernel.sh` script uses an environment module system
(e.g. lmod) to automatically bench the 23 tests on a given set of compilers.

The `bench_to_table.py` Python script allows to filter and convert the results
into an HTML or Markdown table.


### Global results
The following table contains the measured cycles per execution (lower is better)
of the kernel together with the calculations applied to its results,
on an Intel Xeon E5-2650 v4 @ 2.20GHz:

|  | **GNU** | | | | | **Clang** | | | **Intel** | |
|-|-|-|-|-|-|-|-|-|-|-|
|  | **4.9.4** | **5.5.0** | **6.4.0** | **7.3.0** | **8.1.0** | **3.8.0** | **5.0.0** | **6.0.0** | **17.0.2** | **18.0.3** |
| **1** | 55.1 | 55.5 | 54.2 | 54.5 | 32.8 | 65.0 | 58.7 | 55.6 | 32.4 | 32.5 |
| **2** | 57.8 | 58.4 | 57.4 | 77.2 | 32.5 | 64.8 | 62.6 | 60.5 | 32.5 | 32.5 |
| **3** | 75.3 | 73.9 | 71.9 | 72.9 | 71.9 | 67.5 | 63.4 | 60.1 | 32.4 | 32.6 |
| **4** | 56.7 | 57.5 | 56.3 | 74.5 | 32.5 | 66.4 | 60.2 | 57.5 | 32.5 | 32.4 |
| **5** | 74.6 | 73.8 | 73.7 | 74.5 | 74.9 | 65.4 | 62.1 | 59.3 | 32.5 | 32.5 |
| **6** | 74.6 | 73.8 | 73.5 | 74.5 | 74.9 | 64.9 | 62.1 | 58.6 | 32.5 | 32.4 |
| **7** | 55.2 | 56.0 | 55.4 | 73.5 | 32.6 | 63.8 | 61.1 | 57.3 | 32.5 | 32.4 |
| **8** | 55.9 | 58.0 | 55.8 | 74.6 | 32.5 | 64.8 | 61.4 | 57.8 | 32.4 | 32.5 |
| **9** | 55.9 | 56.8 | 56.0 | 73.1 | 32.6 | 64.8 | 61.9 | 59.5 | 32.4 | 32.6 |
| **10** | 77.8 | 76.0 | 76.7 | 77.2 | 76.0 | 82.1 | 79.8 | 75.9 | 32.5 | 32.6 |
| **11** | 83.8 | 79.1 | 79.0 | 79.5 | 78.2 | 84.5 | 81.5 | 77.6 | 33.1 | 77.8 |
| **12** | 56.9 | 56.3 | 55.5 | 55.0 | 54.9 | 63.9 | 60.4 | 55.4 | 56.3 | 56.1 |
| **13** | 75.0 | 75.1 | 76.6 | 83.0 | 74.3 | 81.3 | 79.2 | 75.1 | 32.4 | 32.5 |
| **14** | 61.0 | 60.7 | 60.3 | 78.1 | 80.1 | 67.5 | 66.8 | 63.5 | 77.8 | 77.5 |
| **15** | 79.3 | 78.9 | 77.7 | 78.7 | 33.5 | 67.5 | 65.5 | 63.5 | 32.4 | 32.6 |
| **16** | 75.3 | 73.9 | 71.9 | 72.9 | 71.9 | 67.1 | 62.8 | 59.9 | 32.4 | 32.6 |
| **17** | 63.0 | 63.6 | 62.1 | 76.0 | 32.7 | 70.0 | 65.6 | 61.7 | 32.4 | 32.5 |
| **18** | 60.0 | 62.5 | 60.1 | 75.9 | 32.5 | 66.9 | 64.9 | 61.6 | 32.4 | 32.5 |
| **19** | 65.0 | 65.0 | 64.4 | 78.7 | 32.8 | 74.8 | 70.6 | 66.3 | 32.6 | 32.6 |
| **20** | 66.4 | 66.6 | 65.7 | 80.0 | 78.6 | 76.0 | 72.3 | 67.2 | 77.3 | 77.3 |
| **21** | 73.4 | 68.9 | 65.7 | 36.3 | 36.8 | 82.1 | 73.7 | 70.0 | 81.1 | 34.6 |
| **22** | 78.7 | 77.9 | 77.6 | 77.4 | 77.1 | 83.1 | 81.7 | 77.0 | 79.0 | 76.4 |
| **23** | 76.3 | 68.7 | 66.4 | 36.3 | 36.8 | 81.8 | 72.4 | 70.9 | 80.9 | 34.6 |

(generated by `./bench_to_table.py bench.dat Markdown`)

Some of the tests will be described in the next section.
The test with id 22 is comparable to the full Weno bench of the `C++-NoCopy`
version while the 23th test is for the current "tuned" version.

Note that the calculations applied to the kernel's results are generally light
and implie a penalty of a few cycles.


### GNU/8 and Intel/18 results analysis
In the previous table, the compilers that generate the fastest binaries
are GNU/8.1.0 and Intel/18.0.3.
In this section, we describe some of the tests and the corresponding results
of these two compilers.

#### Tests 1, 4 and 5

Test 1:
```C++
checksum += result.first + result.second;
```

Test 4:
```C++
checksum += result.first <= result.second ? result.first : result.second;
```

Test 5:
```C++
checksum += result.first <= result.second ? 0.5 * result.first : 0.5 * result.second;
```

|  | **GNU/8.1.0** | **Intel/18.0.3** |
|-|-|-|
| **1** | 32.8 | 32.5 |
| **4** | 32.5 | 32.4 |
| **5** | 74.9 | 32.5 |

(generated by `./bench_to_table.py bench.dat Markdown -C -f 'GNU/8|Intel/18' -t '1,4,5'`)

The test 1 is a very simple calculation done on the two Weno reconstructions
and the two compilers performs well on it.
The test 4 and 5 differ only on the `0.5` factor but the GNU compiler's
performance is divided by more than two between this two tests.

#### Tests 9, 10 and 11

Test 9:
```C++
checksum += 0.5 * std::pow( std::min(result.first, result.second), 2);
```

Test 10:
```C++
checksum += result.first <= result.second
    ? ( result.second <= 0
        ? 0.5 * result.second * result.second
        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 ) )
    : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
```

Test 11:
```C++
checksum += 0.5 * std::pow( std::min(result.first, result.second), 2);
checksum += result.first <= result.second
    ? ( result.second <= 0
        ? 0.5 * result.second * result.second
        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 ) )
    : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
```

|  | **GNU/8.1.0** | **Intel/18.0.3** |
|-|-|-|
| **9** | 32.6 | 32.6 |
| **10** | 76.0 | 32.6 |
| **11** | 78.2 | 77.8 |

(generated by `./bench_to_table.py bench.dat Markdown -C -f 'GNU/8|Intel/18' -t '9-11'`)

The 11th test is just a concatenation of tests 9 and 10 but the Intel compiler
fails to generate the same good binary.

#### Tests 14 and 15

Test 14:
```C++
checksum += std::min(
    std::min( 0.5 * result.first * result.first, 0.5 * result.second * result.second ),
    std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second )
);
```

Test 15:
```C++
const auto a = std::min( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
const auto b = std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
checksum += std::min(a, b);
```

|  | **GNU/8.1.0** | **Intel/18.0.3** |
|-|-|-|
| **14** | 80.1 | 77.5 |
| **15** | 33.5 | 32.6 |

(generated by `./bench_to_table.py bench.dat Markdown -C -f 'GNU/8|Intel/18' -t '14,15'`)

Just by using temporaries, the performances are increased by more than a factor 2 !!!

#### Tests 20 and 21

Test 20:
```C++
const auto a = result.second <= 0
            ? 0.5 * result.second * result.second
            : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 );
const auto b = std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second);
const real curr = result.first <= result.second ? a : b;
checksum += curr - last;
last = curr;
```

Test 21:
```C++
const auto a = result.second <= 0
            ? 0.5 * result.second * result.second
            : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 );
const auto b = std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second);
rec[i+1] = result.first <= result.second ? a : b;

...

// After applying the kernel on all discretization points.
for ( std::size_t i = 0; i < size; ++i )
    checksum += rec[i+1] - rec[i];
```

|  | **GNU/8.1.0** | **Intel/18.0.3** |
|-|-|-|
| **20** | 78.6 | 77.3 |
| **21** | 36.8 | 34.6 |

(generated by `./bench_to_table.py bench.dat Markdown -C -f 'GNU/8|Intel/18' -t '20,21'`)

Delaying the difference calculation by storing the reconstruction in
a temporary vector increases the performance by more than a factor 2
(even if it implies using more memory) !!!


# General instructions

### Choose your problem and your numerical flux

You must comment/uncomment the following lines in main.cpp
```
auto const problem = Burghers<Real>{};
//auto const problem = Convection<Real>{};
```
and
```
auto const num_flux = makeGodunovFlux(problem);
//auto const num_flux = makeLaxFriedrichsFlux(problem, 1.);
```

### Plotting:

In main.cpp, uncomment the line
```
#define DO_GNUPLOT_FILES
```
then, at run time, the code will produce a file _resultXXX_ every 100 steps, and
a file _gpfile_. To plot the solution over time, you can use gnuplot:
```
>gnuplot
load "gpfile"
```
But **CAVEAT**: **comment** the line for benchmarking!


### Compilation:
```
mkdir Build
cd Build
cmake ..
make
```
a file "run" is created.
To use an other compiler (eg. clang++) do:
```
CXX=clang++ cmake ..
make
```

### Run the code:

from Build/ directory, type:
```
./run
```
