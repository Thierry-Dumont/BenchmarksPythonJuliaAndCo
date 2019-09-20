#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>

#include <Eigen/Dense>


struct Domain {
    constexpr Domain(double start, double end) : x_start(start), x_end(end) {}
    double x_start;
    double x_end;
};


template <typename Derived>
void init_solution(Eigen::ArrayBase<Derived>& V, const Domain& domain) {
    const std::size_t nx = V.cols();
    const double hx = (domain.x_end - domain.x_start) / nx;
    const double xc = 0.5 * (domain.x_start + domain.x_end) + (nx / 8.0) * hx;

    for(std::size_t i=0; i < nx; ++i) {
        double x = domain.x_start + (i - 0.5) * hx;
        V(0, i) = std::abs(x - xc) < 0.2 ? 1. : 0.;
        V(1, i) = 0.;
    }           
}


void compute_flux(Eigen::Array<double, 2, 1>& flux, double V1, double V2, double tol)
{
    flux(0) = V2;
    if(std::abs(V1) < tol)
        flux(1) = 0.;
    else
        flux(1) = V2 * V2 / V1 + 0.5 * 9.81 * V1 * V1;
}


template<typename ArrayType1d, typename ArrayType2d>
void scheme_LaxFriedrich(ArrayType2d& V, const ArrayType2d& Vold,
                         const ArrayType1d& lambdas,
                         double dt, double dx, double tol)
{
    std::size_t Nx = lambdas.cols();
    double Cx = dt / dx;

    Eigen::Array<double, 2, 1> flux, flux1, flux2;
    compute_flux(flux1, Vold(0, 0), Vold(1, 0), tol);

    V(1, 0) += Cx * (flux1(1) - lambdas(0) * Vold(1, 0));
    for(std::size_t i=0; i<Nx-1; ++i) {
        compute_flux(flux2, Vold(0, i+1), Vold(1, i+1), tol);
        flux = 0.5 * Cx * ((flux2 + flux1) - std::max(lambdas(i), lambdas(i+1)) * (Vold.col(i+1) - Vold.col(i)));
        V.col(i) -= flux;
        V.col(i+1) += flux;
        flux1.swap(flux2);
    }
    V(1, Nx-1) -= Cx * (flux1(1) + lambdas(Nx-1) * Vold(1, Nx-1));
}



template<typename ArrayType1d, typename ArrayType2d>
double updateCFL(ArrayType1d& lambdas, const ArrayType2d& V,
                 double dx, double tol)
{
    std::size_t Nx = lambdas.cols();
    double M = 0.;

    for(std::size_t i=0; i<Nx; ++i) {
        if(std::abs(V(0, i)) < tol) {
            lambdas(i) = 0.;
        }
        else {
            lambdas(i) = std::abs(V(1, i) / V(0, i)) + std::sqrt(9.81 * V(0, i));
            M = std::max(M, lambdas(i));
        }
    }

    return 0.5 * dx / M;
}


template<typename ArrayType1d, typename ArrayType2d>
double update_to_time(ArrayType2d& V, ArrayType2d& Vold, ArrayType1d& lambdas,
                      double t, double tframe, double dx, double tol)
                      
{    
    while(t < tframe) {
        double dt = std::min(updateCFL(lambdas, V, dx, tol),
                             tframe - t);
        Vold = V;
        scheme_LaxFriedrich(V, Vold, lambdas, dt, dx, tol);
        
        t += dt;
    }

    return t;
}



int main() {
    constexpr Domain domain(0., 1.);
    constexpr std::size_t Nx = 16384;
    constexpr double T = 2.0;
    constexpr double dx = (domain.x_end - domain.x_start) / Nx;
    constexpr double tol = 1e-15;


    Eigen::Array<double, 2, Eigen::Dynamic> V(2, Nx);
    init_solution(V, domain);

    Eigen::Array<double, 2, Eigen::Dynamic> Vold(2, Nx);
    Eigen::Array<double, 1, Eigen::Dynamic> lambdas(Nx);
    
    double t = 0;
    std::cout << "Initial time t = " << t << std::endl;

    auto time_start = std::chrono::high_resolution_clock::now();
    t = update_to_time(V, Vold, lambdas, t, T, dx, tol);
    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = time_end - time_start;
    
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;    
    std::cout << "End of simulation, t = " << t << std::endl;
    
    std::cout << "mean(h) = " << V.row(0).mean() << std::endl;

    const Eigen::IOFormat to_file(Eigen::FullPrecision, Eigen::DontAlignCols, "\n", "\n");
    std::ofstream file("sol-cpu");
    file << V.row(0).format(to_file);
    
    return 0;
}
