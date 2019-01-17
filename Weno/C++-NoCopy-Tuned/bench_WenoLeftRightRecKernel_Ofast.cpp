/*
 * Benchs and experiments on the Weno kernel using the -Ofast compiler flag.
 */

#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>

#include "Weno.hpp"

#include "GodunovFlux.hpp"
#include "Burghers.hpp"

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

    const std::size_t size = std::stoull(argv[1]); ///< Virtual domain size
    const std::size_t nrun = argc > 2 ? std::stoull(argv[2]) : 5; ///< Number of run
    const double freq = argc > 3 ? std::stod(argv[3]) : 2200; ///< CPU frequency
    const real v_init = argc > 4 ? std::stod(argv[4]) : 0; ///< initial value

    // Define problem
    auto const problem = Burghers<real>{};

    // Define numerical flux
    auto const num_flux = makeGodunovFlux(problem);

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

        tic();
        for (std::size_t i = 0; i < size; ++i)
        {
            //const auto result = details::WenoLeftRightRecKernel(data[i], data[i+1], data[i+2], data[i+3], data[i+4], data[i+5]);
            const auto result = details::WenoLeftRightRecKernel(&data[i]);

            //checksum += result.first + result.second; // (1) 32.5 @ 256b
            //checksum += std::max(0.5 * result.first * result.first, 0.5 * result.second * result.second); // (2) 32.6 @ 256b
            //checksum += num_flux(result.first, result.second); // (3) 78.2 @ 128b
            checksum += result.first <= result.second ? 0.5 * result.first * result.first : 0.5 * result.second * result.second; // (4) 32.6 @ 256b

            /*
            checksum += result.first <= result.second
                ? ( result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : 0 ) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (5) 73.9 @ 128b
            */

            /*
            checksum += result.first <= result.second
                ? 0.5 * result.second * result.second
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (6) 32.8 @ 256b
            */

            /*
            checksum += result.first <= result.second
                ? ( result.second <= 0 ? 0.5 * result.second * result.second : 0 )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (7) 73.8 @ 128b
            */

            /*
            checksum += result.first <= result.second
                ? ( result.second <= 0 ? 0.5 * result.second * result.second : 0.5 * result.first * result.first )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (8) 33.0 @ 256b
            */

            /*
            checksum += result.first <= result.second
                ? ( result.second <= 0 ? 0.5 * result.second * result.second : std::max(real(0), result.first) + std::min(real(0), -result.first) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (9) 33.1 @ 256b
            */

            /*
            checksum += result.first <= result.second
                ? ( result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : std::max(real(0), result.first) + std::min(real(0), -result.first) ) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (10) 35.2 @ 256b
            */

            /*
            const real curr = result.first <= result.second
                ? ( result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : std::max(real(0), result.first) + std::min(real(0), -result.first) ) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (11) GNU 8.1: 35.2 @ 256b ; Intel 18.0.3: 35.4 @ 256b
            checksum += curr;
            last = curr;
            */

            /*
            const real curr = result.first <= result.second
                ? ( result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : std::max(real(0), result.first) + std::min(real(0), -result.first) ) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (12) GNU 8.1: 73.3 @ 128b ; Intel 18.0.3: 83.0 @ 128b
            checksum += curr - last;
            last = curr;
            */

            /*
            const real curr = result.first <= result.second
                ? ( result.second <= 0
                        ? 0.5 * result.second * result.second
                        : ( result.first >= 0 ? 0.5 * result.first * result.first : std::max(real(0), result.first) + std::min(real(0), -result.first) ) )
                : std::max( 0.5 * result.first * result.first, 0.5 * result.second * result.second ); // (13) GNU 8.1: 36.9 @ 256b ; Intel 18.0.3: 36.7 @ 256b
            rec[i+1] = curr;
            */
        }

        /*
        for ( std::size_t i = 0; i < size; ++i )
            checksum += rec[i+1] - rec[i]; // /!\ (13)
        */

        const double duration = toc();

        std::cout << "\tDuration: "     << duration << " s" << std::endl;
        std::cout << "\tPerformance: "  << size/duration << " it/s" << std::endl;
        std::cout << "\tPerformance: "  << (duration*freq*1e6)/size << " cycle/it" << std::endl;
        std::cout << "\tChecksum: "     << checksum << std::endl;
    }


    return 0;
}
