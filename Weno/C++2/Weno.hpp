#include <utility>
#include <cmath>

namespace {

template <typename T>
inline
T WenoLeftRecKernel(T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    // Regularity indicators
    const T S1 = 13./12. * std::pow(vm3 - 2*vm2 + vm1, 2) + 1./4. * std::pow(  vm3 - 4*vm2 + 3*vm1, 2);
    const T S2 = 13./12. * std::pow(vm2 - 2*vm1 + vp1, 2) + 1./4. * std::pow(  vm2         -   vp1, 2);
    const T S3 = 13./12. * std::pow(vm1 - 2*vp1 + vp2, 2) + 1./4. * std::pow(3*vm1 - 4*vp1 +   vp2, 2);

    // Weighted coefficients for the reconstruction
    const T eps = 1e-6;
    const T w1 = (1./10.) / std::pow(S1 + eps, 2);
    const T w2 = (3./5.)  / std::pow(S2 + eps, 2);
    const T w3 = (3./10.) / std::pow(S3 + eps, 2);

    // Coefficients sum
    const T wsum = w1 + w2 + w3;

    // Reconstruction
    return
          w1 * ( 2./6. * vm3 - 7./6. * vm2 + 11./6. * vm1)
        + w2 * (-1./6. * vm2 + 5./6. * vm1 +  2./6. * vp1)
        + w3 * ( 2./6. * vm1 + 5./6. * vp1 -  1./6. * vp2);
}

template <typename T>
inline
T WenoRightRecKernel(T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    // Regularity indicators
    const T S1 = 13./12. * std::pow(vp3 - 2*vp2 + vp1, 2) + 1./4. * std::pow(  vp3 - 4*vp2 + 3*vp1, 2);
    const T S2 = 13./12. * std::pow(vp2 - 2*vp1 + vm1, 2) + 1./4. * std::pow(  vp2         -   vm1, 2);
    const T S3 = 13./12. * std::pow(vp1 - 2*vm1 + vm2, 2) + 1./4. * std::pow(3*vp1 - 4*vm1 +   vm2, 2);

    // Weighted coefficients for the reconstruction
    const T eps = 1e-6;
    const T w1 = (1./10.) / std::pow(S1 + eps, 2);
    const T w2 = (3./5.)  / std::pow(S2 + eps, 2);
    const T w3 = (3./10.) / std::pow(S3 + eps, 2);

    // Coefficients sum
    const T wsum = w1 + w2 + w3;

    // Reconstruction
    return
          w1 * ( 2./6. * vp3 - 7./6. * vp2 + 11./6. * vp1)
        + w2 * (-1./6. * vp2 + 5./6. * vp1 +  2./6. * vm1)
        + w3 * ( 2./6. * vp1 + 5./6. * vm1 -  1./6. * vm2);
}

template <typename T>
inline
std::pair<T, T> WenoRecKernel(T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    return {
        WenoLeftRecKernel(vm3, vm2, vm1, vp1, vp2, vp3), 
        WenoRightRecKernel(vm3, vm2, vm1, vp1, vp2, vp3)
    };
}

template <typename Flux, typename T>
inline
T WenoFluxKernel(Flux const& flux, T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    const auto rec = WenoRecKernel(vm3, vm3, vm1, vp1, vp2, vp3);
    return flux(rec.first, rec.second);
}

} // namespace

template <typename Flux, typename Real>
class Weno
{
private:
    Flux flux;
    Real length;

public:
    Weno(Flux flux, Real length)
        : flux(std::forward<Flux>(flux)) // TODO: verif
        , length(length)
    {
    }

    Weno(Weno const&) = delete;
    Weno(Weno &&) = delete;
    Weno& operator= (Weno const&) = delete;
    Weno& operator= (Weno &&) = delete;

    ~Weno() = default;

    template <typename In, typename Out>
    void operator() (In const& data, Out & result)
    {
        std::size_t const size = data.size();
        Real const h1 = -1. / (length/size);

        // Left border
        Real left_num_flux = WenoFluxKernel(flux, data[size-3], data[size-2], data[size-1], data[0], data[1], data[2]);
        Real right_num_flux = WenoFluxKernel(flux, data[size-2], data[size-1], data[0], data[1], data[2], data[3]);
        result[0] = h1 * (right_num_flux - left_num_flux); 
    }
};

template <typename Flux, typename Real>
Weno<Flux, Real> makeWeno(Flux && flux, Real length)
{
    return {std::forward<Flux>(flux), length};
}
