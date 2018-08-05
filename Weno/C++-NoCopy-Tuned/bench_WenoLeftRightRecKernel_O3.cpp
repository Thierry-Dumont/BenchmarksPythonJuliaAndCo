/*
 * Benchs and experiments on the Weno kernel using the -O3 compiler flag.
 */

#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <list>
#include <numeric>

// Checksum calculation method
#ifndef TEST
#define TEST 1
#endif

#if TEST == 22
#define USE_ORIGINAL_VERSION
#endif

#include "Weno.hpp"
#include "GodunovFlux.hpp"
#include "Burghers.hpp"

#include "bench_checksum.hpp"


/// Timer used in tic & toc
auto tic_timer = std::chrono::high_resolution_clock::now();

/// Launching the timer
void tic()
{
    tic_timer = std::chrono::high_resolution_clock::now();
}

/// Stopping the timer and returning the duration in seconds
double toc()
{
    const auto toc_timer = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> time_span = toc_timer - tic_timer;
    return time_span.count();
}

int main(int argc, char* argv[])
{
    using real = double;

    // Command-line parameters
    if (argc < 2 || argc > 10)
    {
        std::cerr << "Usage: " << argv[0] << " SIZE [NRUN=5] [FREQ=2200] [V=0]" << std::endl;
        return 1;
    }

    std::cout << "Test #" << TEST << std::endl << std::endl;

    const std::size_t size = std::stoull(argv[1]);                  // Virtual domain size
    const std::size_t nrun = argc > 2 ? std::stoull(argv[2]) : 5;   // Number of run
    const double freq = argc > 3 ? std::stod(argv[3]) : 2200;       // CPU frequency
    const real v_init = argc > 4 ? std::stod(argv[4]) : 0;          // initial value

    // Define problem
    auto const problem = Burghers<real>{};

    // Define numerical flux
    auto const num_flux = makeGodunovFlux(problem);

    // Runs
    std::list<double> duration_list{};

    for (std::size_t run = 0; run < nrun; ++run)
    {
        std::cout << "Run #" << run << std::endl;

        std::vector<real> data(size+6);
        //std::iota(data.begin(), data.end(), v_init);
        std::mt19937 gen(0);
        std::uniform_real_distribution<real> dist(0., 1.);
        std::generate(data.begin(), data.end(), [&] () {return dist(gen);});

        real checksum = 0;
        real last = 0;
        std::vector<real> rec(size+1);
        std::vector<std::pair<real,real>> result(size);

        tic();
        for (std::size_t i = 0; i < size; ++i)
        {
            const auto result = details::WenoLeftRightRecKernel(data[i], data[i+1], data[i+2], data[i+3], data[i+4], data[i+5]);
            //const auto result = details::WenoLeftRightRecKernel(&data[i]);
            //result[i] = details::WenoLeftRightRecKernel(&data[i]);

#if   TEST ==  1
            //  (1) GNU 8.1: 32.5 @ 256b ; Intel 18.0.3: 32.41 @ 256b
            checksum += result.first + result.second;
#elif TEST ==  2
            //  (2) GNU 8.1: 32.6 @ 256b ; Intel 18.0.3: 32.5 @ 256b
            checksum += std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second);
#elif TEST ==  3
            //  (3) GNU 8.1: 75.49 @ 128b ; Intel 18.0.3: 32.5 @ 256b
            checksum += result.first <= result.second
                ? 0.5 * result.first * result.first
                : 0.5 * result.second * result.second;
#elif TEST ==  4
            //  (4) GNU 8.1: 32.6 @ 256b ; Intel 18.0.3: 32.4 @ 256b
            checksum += result.first <= result.second ? result.first : result.second;
#elif TEST ==  5
            //  (5) GNU 8.1: 75.4 @ 128b ; Intel 18.0.3: 32.4 @ 256b
            checksum += result.first <= result.second ? 0.5 * result.first : 0.5 * result.second;
#elif TEST ==  6
            //  (6) GNU 8.1: 76.7 @ 128b ; Intel 18.0.3: 32.4 @ 256b
            checksum += 0.5 * (result.first <= result.second ? result.first : result.second);
#elif TEST ==  7
            //  (7) GNU 8.1: 32.6 @ 256b ; Intel 18.0.3: 32.4 @ 256b
            checksum += 0.5 * std::min(result.first, result.second);
#elif TEST ==  8
            //  (8) GNU 8.1: 32.5 @ 256b ; Intel 18.0.3: 32.4 @ 256b
            checksum += std::min(0.5 * result.first, 0.5 * result.second);
#elif TEST ==  9
            //  (9) GNU 8.1: 32.5 @ 256b ; Intel 18.0.3: 32.4 @ 256b
            // Same as (5) !!
            checksum += 0.5 * std::pow( std::min(result.first, result.second), 2);
#elif TEST == 10
            // (10) GNU 8.1: 78.2 @ 128b ; Intel 18.0.3: 36.7 @ 256b
            checksum += result.first <= result.second
                ? ( result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 ) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
#elif TEST == 11
            // (11) GNU 8.1: 79.5 @ 128b ; Intel 18.0.3: 77.1 @ 128b
            // Same as (9) + (10) !!!
            checksum += 0.5 * std::pow( std::min(result.first, result.second), 2);
            checksum += result.first <= result.second
                ? ( result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 ) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
#elif TEST == 12
            // (12) GNU 8.1: 58.8 @ 128b ; Intel 18.0.3: 61.6 @ 128b
            // Same as (1) !??
            checksum += checksum1(result.first, result.second);
#elif TEST == 13
            // (13) GNU 8.1: 76.4 @ 128b ; Intel 18.0.3: 32.5 @ 256b
            checksum += result.first <= result.second
                ? 0.5 * result.second * result.second
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
#elif TEST == 14
            // (14) GNU 8.1: 81.5 @ 128b ; Intel 18.0.3: 77.5 @ 128b
            checksum += std::min(
                std::min( 0.5 * result.first * result.first, 0.5 * result.second * result.second ),
                std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second )
            );
#elif TEST == 15
            // (15) GNU 8.1: 33.8 @ 256b ; Intel 18.0.3: 32.5 @ 256b
            // Same as (14) !!!!!!!!
            const auto a = std::min( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
            const auto b = std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second );
            checksum += std::min(a, b);
#elif TEST == 16
            // (16) GNU 8.1: 75.7 @ 128b ; Intel 18.0.3: 32.4 @ 256b
            // Same as (3)
            const auto a = 0.5 * result.first * result.first;
            const auto b = 0.5 * result.second * result.second;
            checksum += result.first <= result.second ? a : b;
#elif TEST == 17
            // (17) GNU 8.1: 32.7 @ 256b ; Intel 18.0.3: 32.5 @ 256b
            // Same as (3) and (16) !!!
            const auto a = -std::min(-0.5 * result.first * result.first, 0.);
            const auto b = std::max(0., 0.5 * result.second * result.second);
            checksum += result.first <= result.second ? a : b;
#elif TEST == 18
            // (18) GNU 8.1: 32.6 @ 256b ; Intel 18.0.3: 32.5 @ 256b
            // Same as (13) !!
            const auto a = 0.5 * result.second * result.second;
            const auto b = std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second);
            checksum += result.first <= result.second ? a : b;
#elif TEST == 19
            // (19) GNU 8.1: 32.7 @ 256b ; Intel 18.0.3: 32.6 @ 256b
            // Same as (10) !!!
            const auto a = result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 );
            const auto b = std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second);
            checksum += result.first <= result.second ? a : b;
#elif TEST == 20
            // (20) GNU 8.1: 81.9 @ 128b ; Intel 18.0.3: 77.0 @ 128b
            // Just a swap added to (19)...
            const auto a = result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 );
            const auto b = std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second);
            const real curr = result.first <= result.second ? a : b;
            checksum += curr - last;
            last = curr;
#elif TEST == 21
            // (21) GNU 8.1: 36.7 @ 256b ; Intel 18.0.3: 34.7 @ 128b
            // Same as (20) but using a temporary vector to store the result !!
            const auto a = result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 );
            const auto b = std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second);
            rec[i+1] = result.first <= result.second ? a : b;
#elif TEST == 22
            // (22) GNU 8.1: 79.8 @ 128b ; Intel 18.0.3: 78.1 @ 128b
            // Same as (21) but using the original Weno bench classes
            const real curr = num_flux(result.first, result.second);
            checksum += curr - last;
            last = curr;
#elif TEST == 23
            // (23) GNU 8.1: 36.7 @ 256b ; Intel 18.0.3: 34.9 @ 256b
            // Same as (21) but using the tuned Weno bench classes
            rec[i+1] = num_flux(result.first, result.second);
#else
#error Invalid TEST id !
#endif
        }

#if TEST == 21 || TEST == 23
        for ( std::size_t i = 0; i < size; ++i )
            checksum += rec[i+1] - rec[i];
#endif


        const double duration = toc();
        duration_list.push_back(duration);


        std::cout << "\tDuration: "     << duration << " s" << std::endl;
        std::cout << "\tPerformance: "  << size/duration << " it/s" << std::endl;
        std::cout << "\tPerformance: "  << (duration*freq*1e6)/size << " cycle/it" << std::endl;
        std::cout << "\tChecksum: "     << checksum << std::endl;
    }

    // Summary

    // Sanitizing durations
    if (duration_list.size() >= 3)
    {
        duration_list.sort();
        duration_list.pop_front();
        duration_list.pop_back();
    }

    // Mean result
    const double duration = std::accumulate(duration_list.cbegin(), duration_list.cend(), 0.) / duration_list.size();

    std::cout << "Summary:" << std::endl;
    std::cout << "\tDuration: "     << duration << " s" << std::endl;
    std::cout << "\tPerformance: "  << size/duration << " it/s" << std::endl;
    std::cout << "\tPerformance: "  << (duration*freq*1e6)/size << " cycle/it" << std::endl;

    // Robot format
    std::cout << std::endl;
    std::cout << TEST << " "
              << duration << " "
              << size/duration << " "
              << (duration*freq*1e6)/size << std::endl;

    return 0;
}
