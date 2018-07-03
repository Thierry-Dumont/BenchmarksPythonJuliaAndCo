#pragma once

#include <utility>
#include <type_traits>
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
    return (
          w1 * ( 2./6. * vm3 - 7./6. * vm2 + 11./6. * vm1)
        + w2 * (-1./6. * vm2 + 5./6. * vm1 +  2./6. * vp1)
        + w3 * ( 2./6. * vm1 + 5./6. * vp1 -  1./6. * vp2)
    ) / wsum;
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
    return (
          w1 * ( 2./6. * vp3 - 7./6. * vp2 + 11./6. * vp1)
        + w2 * (-1./6. * vp2 + 5./6. * vp1 +  2./6. * vm1)
        + w3 * ( 2./6. * vp1 + 5./6. * vm1 -  1./6. * vm2)
    ) / wsum;
}

template <typename T>
inline
std::pair<T, T> WenoLeftRightRecKernel(T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    const T eps = 1e-6;

    // Regularity indicators (common part)
    const T S_vm2 = 13./12. * std::pow(vm3 - 2*vm2 + vm1, 2);
    const T S_vm1 = 13./12. * std::pow(vm2 - 2*vm1 + vp1, 2);
    const T S_vp1 = 13./12. * std::pow(vm1 - 2*vp1 + vp2, 2);
    const T S_vp2 = 13./12. * std::pow(vp1 - 2*vp2 + vp3, 2);

    // Regularity indicators
    const T S1_left  = S_vm2 + 1./4. * std::pow(  vm3 - 4*vm2 + 3*vm1, 2);
    const T S2_left  = S_vm1 + 1./4. * std::pow(  vm2         -   vp1, 2);
    const T S3_left  = S_vp1 + 1./4. * std::pow(3*vm1 - 4*vp1 +   vp2, 2);
    const T S1_right = S_vp2 + 1./4. * std::pow(  vp3 - 4*vp2 + 3*vp1, 2);
    const T S2_right = S_vp1 + 1./4. * std::pow(  vp2         -   vm1, 2);
    const T S3_right = S_vm1 + 1./4. * std::pow(3*vp1 - 4*vm1 +   vm2, 2);

    // Weighted coefficients for the reconstruction
    const T w1_left  = (1./10.) / std::pow(S1_left + eps, 2);
    const T w2_left  = (3./5.)  / std::pow(S2_left + eps, 2);
    const T w3_left  = (3./10.) / std::pow(S3_left + eps, 2);
    const T w1_right = (1./10.) / std::pow(S1_right + eps, 2);
    const T w2_right = (3./5.)  / std::pow(S2_right + eps, 2);
    const T w3_right = (3./10.) / std::pow(S3_right + eps, 2);

    // Coefficients sum
    const T wsum_left = w1_left + w2_left + w3_left;
    const T wsum_right = w1_right + w2_right + w3_right;

    // Reconstruction
    const T rec_left = (
          w1_left * ( 2./6. * vm3 - 7./6. * vm2 + 11./6. * vm1)
        + w2_left * (-1./6. * vm2 + 5./6. * vm1 +  2./6. * vp1)
        + w3_left * ( 2./6. * vm1 + 5./6. * vp1 -  1./6. * vp2)
    ) / wsum_left;
    
    const T rec_right = (
          w1_right * ( 2./6. * vp3 - 7./6. * vp2 + 11./6. * vp1)
        + w2_right * (-1./6. * vp2 + 5./6. * vp1 +  2./6. * vm1)
        + w3_right * ( 2./6. * vp1 + 5./6. * vm1 -  1./6. * vm2)
    ) / wsum_right;

    return {rec_left, rec_right};
}

template <typename T>
inline
std::pair<T, T> WenoRecKernel(T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    /*
    return {
        WenoLeftRecKernel(vm3, vm2, vm1, vp1, vp2, vp3), 
        WenoLeftRecKernel(vp3, vp2, vp1, vm1, vm2, vm3), 
        //WenoRightRecKernel(vm3, vm2, vm1, vp1, vp2, vp3)
    };
    */
    return WenoLeftRightRecKernel(vm3, vm2, vm1, vp1, vp2, vp3);
}

template <typename Flux, typename T>
inline
T WenoFluxKernel(Flux const& flux, T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    const auto rec = WenoRecKernel(vm3, vm3, vm1, vp1, vp2, vp3);
    return flux(rec.first, rec.second);
}

} // namespace

template <typename Flux, typename TReal>
class Weno
{
public:
    using Real = TReal;

private:
    Flux flux;
    Real length;

public:
    template <typename TFlux>
    Weno(TFlux && flux, Real length)
        : flux(std::forward<TFlux>(flux))
        , length(length)
    {
    }

    template <typename In, typename Out>
    void operator() (In const& data, Out & result) const
    {
        std::size_t const size = data.size();
        Real const h1 = -1. / (length/size);

        // Diff helper
        struct Helper
        {
            Real left;
            Real operator() (Real right) { Real diff = right - left; left = right; return diff; }
        };

        // Left border
        Helper helper = { WenoFluxKernel(flux, data[size-3], data[size-2], data[size-1], data[0], data[1], data[2]) };
        result[0] = h1 * helper(WenoFluxKernel(flux, data[size-2], data[size-1], data[0], data[1], data[2], data[3]));
        result[1] = h1 * helper(WenoFluxKernel(flux, data[size-1], data[0], data[1], data[2], data[3], data[4]));

        // Center part
        for (std::size_t pos = 2; pos < size-3; ++pos)
            result[pos] = h1 * helper(WenoFluxKernel(flux, data[pos-2], data[pos-1], data[pos], data[pos+1], data[pos+2], data[pos+3]));

        // Right border
        result[size-3] = h1 * helper(WenoFluxKernel(flux, data[size-5], data[size-4], data[size-3], data[size-2], data[size-1], data[0]));
        result[size-2] = h1 * helper(WenoFluxKernel(flux, data[size-4], data[size-3], data[size-2], data[size-1], data[0], data[1]));
        result[size-1] = h1 * helper(WenoFluxKernel(flux, data[size-3], data[size-2], data[size-1], data[0], data[1], data[2]));
    }
};

template <typename Flux, typename Real>
auto makeWeno(Flux && flux, Real length)
{
    return Weno<Flux, Real>(std::forward<Flux>(flux), length);
}
