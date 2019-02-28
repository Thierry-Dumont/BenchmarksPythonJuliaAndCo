#include <chrono>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <limits.h>
#include <typeinfo>
#include <utility>
#include "GodunovFlux.hpp"
#include "LaxFriedrichsFlux.hpp"
#include "Burghers.hpp"
#include "Convection.hpp"
#include "Weno.hpp"
#include "RK3TVD.hpp"

//#define DO_GNUPLOT_FILES

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


/// Returning hostname
std::string host()
{
  char hostnameC[HOST_NAME_MAX];
  gethostname(hostnameC, HOST_NAME_MAX);
  return std::string(hostnameC);
}
template<class T> std::pair<std::string,std::string> how_computed(T& t)
{
  std::string s=typeid(t).name();
  std::string problem=s.find("Burghers")!=std::string::npos? "Burghers":"Convection";
  std::string flux=s.find("Godunov")!=std::string::npos?"Godunov": "Lax-Friedrichs";
  return std::make_pair(problem,flux);
}
/// Initial condition
template <typename TData, typename T>
void init(TData & X, T L)
{
    std::size_t const size = X.size();
    T const h = L / size;

    for ( std::size_t i = 0; i < size; i++)
    {
        if (i > size/8 && i < size/2 + size/8)
            X[i] = T(1.) - 2*(i - size/8) * h/L;
        else
            X[i] = 0.;
    }
}

/// Main program entry
int main()
{
    using std::cout;
    using std::endl;

    // Real type
    using Real = double;

    // Simulation parameters
    std::size_t const size = 1000;
    Real const L = 1.0;
    Real const dt = Real(0.8)/size;
    Real const T = 1.0;

    // Define problem
    auto const problem = Burghers<Real>{};
    //auto const problem = Convection<Real>{};
    
    // Define numerical flux
    auto const num_flux = makeGodunovFlux(problem);
    //auto const num_flux = makeLaxFriedrichsFlux(problem, 1.);

    // Scheme
    auto const scheme = makeWeno(num_flux, L);

    //std::cout<<typeid(scheme).name()<<endl;
    // Time integrator
    auto const time_int = makeRK3TVD(scheme);

    // Initialization
    auto const hostname = host();
    std::cout << "Hostname: " << hostname << std::endl;

    auto P=how_computed(num_flux);
    std::cout<<P.first <<" with "<<P.second<<endl;
    
    std::size_t const nsteps = T/dt;
    std::cout << "size = " << size << " ; dt = " << dt << " ; nsteps = " << nsteps << std::endl;
    
    std::vector<Real> X(size);
    init(X, L);

#ifdef DO_GNUPLOT_FILES
    std::size_t const ngp = std::max<std::size_t>(1, nsteps/100);
    cout << endl << "We are going to create files to visualize the solution: ";
    cout << "DISABLE THiS FOR BENCHMARKING!" << endl << endl;
    cout << "One plot every " << ngp << " steps. We will do about " << nsteps
         << " steps." << endl;
    std::ofstream f, gpfile;
    gpfile.open("gpfile");
#endif

    // Main loop
    Real t = 0.;
    int step = 0;
    tic();

    while(t < T)
    {
        time_int.step(X, dt);
      
#ifdef DO_GNUPLOT_FILES
        if (step % ngp == 0)
        {
            gpfile << "plot " << "\"results" + std::to_string(step) + "\" with lines" << endl;
            gpfile << "pause 0.1" << endl;
            f.open("results" + std::to_string(step));
            for (std::size_t i = 0; i < size; i++)
                f << X[i] << endl;
            f.close();
        }
 #endif

      ++step;
      t += dt;
    }
    
    auto const duration = toc();

    cout << "Computing time: " << duration << " seconds."<<endl;
    cout << "nsteps: " << step << endl;

#ifdef DO_GNUPLOT_FILES 
    gpfile.close();
#else
    // Make one file, with the final computation
    std::ofstream f;
    f.open("gp");
    for ( std::size_t i = 0; i < size; i++)
        f << X[i] << endl;
    f.close();

    cout << "A file 'gp' with the final solution was created." << endl; 
#endif

    std::ofstream fb; fb.open("../RunningOn" + hostname);
    fb<<P.first<<" "<<P.second<<endl;
    fb << duration << endl;
    fb.close();

    cout << "end" << endl;
    return EXIT_SUCCESS;
}
