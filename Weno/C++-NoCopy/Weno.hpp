#pragma once

#include <utility>
#include <type_traits>
#include <cmath>
#include <numeric>
#include <array>
#include <immintrin.h>

namespace {

#if !defined(USE_FMA) && !defined(USE_INTRINSIC1) && !defined(USE_INTRINSIC2)
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
    const T S1_right = S_vp2 + 1./4. * std::pow(3*vp1 - 4*vp2 +   vp3, 2);
    const T S2_right = S_vp1 + 1./4. * std::pow(  vm1         -   vp2, 2);
    const T S3_right = S_vm1 + 1./4. * std::pow(  vm2 - 4*vm1 + 3*vp1, 2);

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
          w1_right * ( 11./6. * vp1 - 7./6. * vp2 + 2./6. * vp3)
        + w2_right * (  2./6. * vm1 + 5./6. * vp1 - 1./6. * vp2)
        + w3_right * ( -1./6. * vm2 + 5./6. * vm1 + 2./6. * vp1)
    ) / wsum_right;

    return {rec_left, rec_right};
}

#endif

#ifdef USE_FMA
template <typename T>
inline
std::pair<T, T> WenoLeftRightRecKernel(T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    const T eps = 1e-6;

    // Regularity indicators (common part)
    T S_vm2 = std::fma(T(-2), vm2, vm1); // -2*vm2 + vm1
    T S_vm1 = std::fma(T(-2), vm1, vp1); // -2*vm1 + vp1
    T S_vp1 = std::fma(T(-2), vp1, vp2); // -2*vp1 + vp2
    T S_vp2 = std::fma(T(-2), vp2, vp3); // -2*vp2 + vp3

    S_vm2 = std::fma(T(1), vm3, S_vm2); // vm3 + (-2*vm2 + vm1)
    S_vm1 = std::fma(T(1), vm2, S_vm1); // vm2 + (-2*vm1 + vp1)
    S_vp1 = std::fma(T(1), vm1, S_vp1); // vm1 + (-2*vp1 + vp2)
    S_vp2 = std::fma(T(1), vp1, S_vp2); // vp1 + (-2*vp2 + vp3)

    S_vm2 = std::pow(S_vm2, 2); // std::pow(..., 2)
    S_vm1 = std::pow(S_vm1, 2); // std::pow(..., 2)
    S_vp1 = std::pow(S_vp1, 2); // std::pow(..., 2)
    S_vp2 = std::pow(S_vp2, 2); // std::pow(..., 2)

    /*
    const T S_vm2 = 13./12. * std::pow(vm3 - 2*vm2 + vm1, 2);
    const T S_vm1 = 13./12. * std::pow(vm2 - 2*vm1 + vp1, 2);
    const T S_vp1 = 13./12. * std::pow(vm1 - 2*vp1 + vp2, 2);
    const T S_vp2 = 13./12. * std::pow(vp1 - 2*vp2 + vp3, 2);
    */

    // Regularity indicators
    T S1_left = std::fma(T(-4), vm2, vm3); // vm3 - 4*vm2
    T S2_left = std::fma(T(-1), vp1, vm2); // vm2 - vp1
    T S3_left = std::fma(T(3), vm1, vp2);  // 3*vm1 + vp2
    T S1_right = std::fma(T(-4), vp2, vp3); // vp3 - 4*vp2
    T S2_right = std::fma(T(-1), vp2, vm1); // vm1 - vp2
    T S3_right = std::fma(T(3), vp1, vm2); // 3*vp1 + vm2

    S1_left = std::fma(T(3), vm1, S1_left); // 3*vm1 + (...)
    S3_left = std::fma(T(-4), vp1, S3_left); // -4*vp1 + (...)
    S1_right = std::fma(T(3), vp1, S1_right); // 3*vp1 + (...)
    S3_right = std::fma(T(-4), vm1, S3_right); // -4*vm1 + (...)

    S1_left = std::pow(S1_left, 2);
    S2_left = std::pow(S2_left, 2);
    S3_left = std::pow(S3_left, 2);
    S1_right = std::pow(S1_right, 2);
    S2_right = std::pow(S2_right, 2);
    S3_right = std::pow(S3_right, 2);

    S1_left = std::fma(T(3./13.), S1_left, S_vm2);
    S2_left = std::fma(T(3./13.), S2_left, S_vm1);
    S3_left = std::fma(T(3./13.), S3_left, S_vp1);
    S1_right = std::fma(T(3./13.), S1_right, S_vp2);
    S2_right = std::fma(T(3./13.), S2_right, S_vp1);
    S3_right = std::fma(T(3./13.), S3_right, S_vm1);

    /*
    const T S1_left  = S_vm2 + 1./4. * std::pow(  vm3 - 4*vm2 + 3*vm1, 2);
    const T S2_left  = S_vm1 + 1./4. * std::pow(  vm2         -   vp1, 2);
    const T S3_left  = S_vp1 + 1./4. * std::pow(3*vm1 - 4*vp1 +   vp2, 2);
    const T S1_right = S_vp2 + 1./4. * std::pow(  vp3 - 4*vp2 + 3*vp1, 2);
    const T S2_right = S_vp1 + 1./4. * std::pow(  vp2         -   vm1, 2);
    const T S3_right = S_vm1 + 1./4. * std::pow(3*vp1 - 4*vm1 +   vm2, 2);
    */

    // Weighted coefficients for the reconstruction
    T w1_left = std::fma(T(13./12.), S1_left, eps);
    T w2_left = std::fma(T(13./12.), S2_left, eps);
    T w3_left = std::fma(T(13./12.), S3_left, eps);
    T w1_right = std::fma(T(13./12.), S1_right, eps);
    T w2_right = std::fma(T(13./12.), S2_right, eps);
    T w3_right = std::fma(T(13./12.), S3_right, eps);

    w1_left = std::pow(w1_left, 2);
    w2_left = std::pow(w2_left, 2);
    w3_left = std::pow(w3_left, 2);
    w1_right = std::pow(w1_right, 2);
    w2_right = std::pow(w2_right, 2);
    w3_right = std::pow(w3_right, 2);

    w1_left = (1./10.) / w1_left;
    w2_left = (3./5.)  / w2_left;
    w3_left = (3./10.) / w3_left;
    w1_right = (1./10.) / w1_right;
    w2_right = (3./5.)  / w2_right;
    w3_right = (3./10.) / w3_right;

    /*
    const T w1_left  = (1./10.) / std::pow(S1_left + eps, 2);
    const T w2_left  = (3./5.)  / std::pow(S2_left + eps, 2);
    const T w3_left  = (3./10.) / std::pow(S3_left + eps, 2);
    const T w1_right = (1./10.) / std::pow(S1_right + eps, 2);
    const T w2_right = (3./5.)  / std::pow(S2_right + eps, 2);
    const T w3_right = (3./10.) / std::pow(S3_right + eps, 2);
    */

    // Coefficients sum
    const T wsum_left = w1_left + w2_left + w3_left;
    const T wsum_right = w1_right + w2_right + w3_right;

    // Reconstruction
    T rec1_left =  2./6. * vm3;
    T rec2_left = -1./6. * vm2;
    T rec3_left =  2./6. * vm1;
    T rec1_right =  2./6. * vp3;
    T rec2_right = -1./6. * vp2;
    T rec3_right =  2./6. * vp1;

    rec1_left = std::fma(-7./6., vm2, rec1_left);
    rec2_left = std::fma( 5./6., vm1, rec2_left);
    rec3_left = std::fma( 5./6., vp1, rec3_left);
    rec1_right = std::fma(-7./6., vp2, rec1_right);
    rec2_right = std::fma( 5./6., vp1, rec2_right);
    rec3_right = std::fma( 5./6., vm1, rec3_right);

    rec1_left = std::fma(11./6., vm1, rec1_left);
    rec2_left = std::fma( 2./6., vp1, rec2_left);
    rec3_left = std::fma(-1./6., vp2, rec3_left);
    rec1_right = std::fma(11./6., vp1, rec1_right);
    rec2_right = std::fma( 2./6., vm1, rec2_right);
    rec3_right = std::fma(-1./6., vm2, rec3_right);

    rec1_left = w1_left * rec1_left;
    rec2_left = w2_left * rec2_left;
    rec3_left = w3_left * rec3_left;
    rec1_right = w1_right * rec1_right;
    rec2_right = w2_right * rec2_right;
    rec3_right = w3_right * rec3_right;

    const T rec_left = (rec1_left + rec2_left + rec3_left) / wsum_left;
    const T rec_right = (rec1_right + rec2_right + rec3_right) / wsum_right;

    /*
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
    */

    return {rec_left, rec_right};
}
#endif

#ifdef USE_INTRINSIC1

inline double sum(__m256d v)
{
    // See https://software.intel.com/en-us/forums/intel-isa-extensions/topic/346695
    __m256d hsum = _mm256_add_pd(v, _mm256_permute2f128_pd(v, v, 0x1));
    __m128d result = _mm_hadd_pd( _mm256_castpd256_pd128(hsum), _mm256_castpd256_pd128(hsum) );
    return result[0];
}

std::pair<double, double> WenoLeftRightRecKernel(double vm3, double vm2, double vm1, double vp1, double vp2, double vp3)
{
    // Regularity indicators (common part)
    __m256d vm3p1   = _mm256_set_pd(vm3, vm2, vm1, vp1);
    __m256d vm2p2   = _mm256_set_pd(vm2, vm1, vp1, vp2);
    __m256d vm1p3   = _mm256_set_pd(vm1, vp1, vp2, vp3);
    __m256d coeff   = _mm256_set_pd(-2, -2, -2, -2);

    __m256d S_vm2p2;
    S_vm2p2 = _mm256_fmadd_pd(coeff, vm2p2, vm1p3);     // -2*vm2p2 + vm1p3
    //S_vm2p1 = _mm256_fmadd_pd(plus1, vm3p1, S_vm2p1);
    S_vm2p2 = _mm256_add_pd(vm3p1, S_vm2p2);            // vm3p1 + (-2*vm2p1 + vm1p3)
    S_vm2p2 = _mm256_mul_pd(S_vm2p2, S_vm2p2);          // std::pow(..., 2)

    // Regularity indicators
    __m256d S123X_left, SX321_right;

    __m256d coeff123X = _mm256_set_pd(1, 1,  3, 0);
    __m256d coeffX321 = _mm256_set_pd(0, 3, -1, 1);
    S123X_left  = _mm256_mul_pd(coeff123X, vm3p1);
    SX321_right = _mm256_mul_pd(coeffX321, vm1p3);

    coeff123X   = _mm256_set_pd(-4, 0, -4, 0);
    coeffX321   = _mm256_set_pd(0, -4, 0, -4);
    S123X_left  = _mm256_fmadd_pd(coeff123X, vm2p2, S123X_left);
    SX321_right = _mm256_fmadd_pd(coeffX321, vm2p2, SX321_right);

    coeff123X   = _mm256_set_pd(3, -1, 1, 0);
    coeffX321   = _mm256_set_pd(0,  1, 1, 3);
    S123X_left  = _mm256_fmadd_pd(coeff123X, vm1p3, S123X_left);
    SX321_right = _mm256_fmadd_pd(coeffX321, vm3p1, SX321_right);

    S123X_left  = _mm256_mul_pd(S123X_left, S123X_left);
    SX321_right = _mm256_mul_pd(SX321_right, SX321_right);

    coeff       = _mm256_set_pd(3./13., 3./13., 3./13., 3./13.);
    S123X_left  = _mm256_fmadd_pd(coeff, S123X_left, S_vm2p2);
    SX321_right = _mm256_fmadd_pd(coeff, SX321_right, S_vm2p2);

    // Weighted coefficients for the reconstruction
    __m256d w123X_left, wX321_right;
    coeff       = _mm256_set_pd(13./12., 13./12., 13./12., 13./12.);
    __m256d eps = _mm256_set_pd(1e-6, 1e-6, 1e-6, 1e-6);
    w123X_left  = _mm256_fmadd_pd(coeff, S123X_left, eps);
    wX321_right = _mm256_fmadd_pd(coeff, SX321_right, eps);

    w123X_left  = _mm256_mul_pd(w123X_left, w123X_left);
    wX321_right = _mm256_mul_pd(wX321_right, wX321_right);

    coeff123X   = _mm256_set_pd(1./10., 3./5., 3./10., 0);
    coeffX321   = _mm256_set_pd(0, 3./10., 3./5., 1./10.);
    w123X_left  = _mm256_div_pd(coeff123X, w123X_left);
    wX321_right = _mm256_div_pd(coeffX321, wX321_right);

    // Reconstruction
    __m256d rec123X_left, recX321_right;
    coeff123X   = _mm256_set_pd(2./6., -1./6., 2./6., 0);
    coeffX321   = _mm256_set_pd(0, 2./6., -1./6., 2./6.);
    rec123X_left    = _mm256_mul_pd(coeff123X, vm3p1);
    recX321_right   = _mm256_mul_pd(coeffX321, vm1p3);

    coeff123X   = _mm256_set_pd(-7./6., 5./6., 5./6., 0);
    coeffX321   = _mm256_set_pd(0, 5./6., 5./6., -7./6.);
    rec123X_left    = _mm256_fmadd_pd(coeff123X, vm2p2, rec123X_left);
    recX321_right   = _mm256_fmadd_pd(coeffX321, vm2p2, recX321_right);

    coeff123X   = _mm256_set_pd(11./6., 2./6., -1./6., 0);
    coeffX321   = _mm256_set_pd(0, -1./6., 2./6., 11./6.);
    rec123X_left    = _mm256_fmadd_pd(coeff123X, vm1p3, rec123X_left);
    recX321_right   = _mm256_fmadd_pd(coeffX321, vm3p1, recX321_right);

    rec123X_left    = _mm256_mul_pd(rec123X_left, w123X_left);
    recX321_right   = _mm256_mul_pd(recX321_right, wX321_right);

    return {
        sum(rec123X_left) / sum(w123X_left),
        sum(recX321_right) / sum(wX321_right)
    };
}
#endif

#ifdef USE_INTRINSIC2

inline double sum(__m256d v)
{
    // See https://software.intel.com/en-us/forums/intel-isa-extensions/topic/346695
    __m256d hsum = _mm256_add_pd(v, _mm256_permute2f128_pd(v, v, 0x1));
    __m128d result = _mm_hadd_pd( _mm256_castpd256_pd128(hsum), _mm256_castpd256_pd128(hsum) );
    return result[0];
}

inline __m256d dot(__m256d x[4], __m256d y[4])
{
    // See https://stackoverflow.com/questions/10454150/intel-avx-256-bits-version-of-dot-product-for-double-precision-floating-point-v
    __m256d xy0 = _mm256_mul_pd( x[0], y[0] );
    __m256d xy1 = _mm256_mul_pd( x[1], y[1] );
    __m256d xy2 = _mm256_mul_pd( x[2], y[2] );
    __m256d xy3 = _mm256_mul_pd( x[3], y[3] );

    // low to high: xy00+xy01 xy10+xy11 xy02+xy03 xy12+xy13
    __m256d temp01 = _mm256_hadd_pd( xy0, xy1 );

    // low to high: xy20+xy21 xy30+xy31 xy22+xy23 xy32+xy33
    __m256d temp23 = _mm256_hadd_pd( xy2, xy3 );

    // low to high: xy02+xy03 xy12+xy13 xy20+xy21 xy30+xy31
    __m256d swapped = _mm256_permute2f128_pd( temp01, temp23, 0x21 );

    // low to high: xy00+xy01 xy10+xy11 xy22+xy23 xy32+xy33
    __m256d blended = _mm256_blend_pd(temp01, temp23, 0b1100);

    __m256d dotproduct = _mm256_add_pd( swapped, blended );
    return dotproduct;
}

// FIXME: there is an error !
std::pair<double, double> WenoLeftRightRecKernel(double vm3, double vm2, double vm1, double vp1, double vp2, double vp3)
{
    // Different shift of the input data
    __m256d v_shift[4] = {
        _mm256_set_pd(vm3, vm2, vm1, 0),
        _mm256_set_pd(vm2, vm1, vp1, 0),
        _mm256_set_pd(vm1, vp1, vp2, 0),
        _mm256_set_pd(vp1, vp2, vp3, 0)
    };

    // Regularity indicators (common part)
    __m256d stencil_S_common[4] = {
         _mm256_set_pd(1, -2, 1, 0),
         _mm256_set_pd(1, -2, 1, 0),
         _mm256_set_pd(1, -2, 1, 0),
         _mm256_set_pd(1, -2, 1, 0)
    };

    __m256d S_vm2p2 = dot(stencil_S_common, v_shift);
    S_vm2p2 = _mm256_mul_pd(S_vm2p2, S_vm2p2);

    // Regularity indicators
    __m256d stencil_S_left[4] = {
         _mm256_set_pd(1, -4,  3, 0),
         _mm256_set_pd(1,  0, -1, 0),
         _mm256_set_pd(3, -4,  1, 0),
         _mm256_set_pd(0,  0,  0, 0)
    };

    __m256d S123X_left = dot(stencil_S_left, v_shift);

    __m256d stencil_S_right[4] = {
         _mm256_set_pd( 0,  0,  0, 0),
         _mm256_set_pd( 1, -4,  3, 0),
         _mm256_set_pd(-1,  0,  1, 0),
         _mm256_set_pd( 3, -4,  1, 0)
    };

    __m256d SX321_right = dot(stencil_S_right, v_shift);

    S123X_left  = _mm256_mul_pd(S123X_left, S123X_left);
    SX321_right = _mm256_mul_pd(SX321_right, SX321_right);

    __m256d coeff   = _mm256_set_pd(3./13., 3./13., 3./13., 3./13.);
    __m256d vm2p2   = _mm256_set_pd(vp2, vp1, vm1, vm2);
    S123X_left      = _mm256_fmadd_pd(coeff, S123X_left, S_vm2p2);
    SX321_right     = _mm256_fmadd_pd(coeff, SX321_right, S_vm2p2);

    // Weighted coefficients for the reconstruction
    __m256d w123X_left, wX321_right;
    __m256d coeff_left  = _mm256_set_pd(13./12., 13./12., 13./12., 0);
    __m256d eps_left    = _mm256_set_pd(1e-6, 1e-6, 1e-6, 0);
    w123X_left  = _mm256_fmadd_pd(coeff_left, S123X_left, eps_left);

    __m256d coeff_right = _mm256_set_pd(0, 13./12., 13./12., 13./12.);
    __m256d eps_right   = _mm256_set_pd(0, 1e-6, 1e-6, 1e-6);
    wX321_right = _mm256_fmadd_pd(coeff_right, SX321_right, eps_right);

    w123X_left  = _mm256_mul_pd(w123X_left, w123X_left);
    wX321_right = _mm256_mul_pd(wX321_right, wX321_right);

    __m256d coeff123X = _mm256_set_pd(1./10., 3./5., 3./10., 0);
    __m256d coeffX321 = _mm256_set_pd(0, 3./10., 3./5., 1./10.);
    w123X_left  = _mm256_div_pd(coeff123X, w123X_left);
    wX321_right = _mm256_div_pd(coeffX321, wX321_right);

    // Reconstruction
    __m256d stencil_rec_left[4] = {
         _mm256_set_pd( 2./6., -7./6.,  11./6., 0),
         _mm256_set_pd(-1./6.,  5./6.,   2./6., 0),
         _mm256_set_pd( 2./6.,  5./6.,  -1./6., 0),
         _mm256_set_pd(0, 0, 0, 0)
    };

    __m256d rec123X_left = dot(stencil_rec_left, v_shift);

    __m256d stencil_rec_right[4] = {
         _mm256_set_pd( 0, 0, 0, 0),
         _mm256_set_pd(-1./6.,  5./6.,   2./6., 0),
         _mm256_set_pd( 2./6.,  5./6.,  -1./6., 0),
         _mm256_set_pd(11./6., -7./6.,   2./6., 0)
    };

    __m256d recX321_right = dot(stencil_rec_right, v_shift);

    rec123X_left    = _mm256_mul_pd(rec123X_left, w123X_left);
    recX321_right   = _mm256_mul_pd(recX321_right, wX321_right);

    return {
        sum(rec123X_left)  / sum(w123X_left),
        sum(recX321_right) / sum(wX321_right)
    };
}
#endif

template <typename T>
inline
std::pair<T, T> WenoRecKernel(T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    return WenoLeftRightRecKernel(vm3, vm2, vm1, vp1, vp2, vp3);
}

template <typename Flux, typename T>
inline
T WenoFluxKernel(Flux const& flux, T vm3, T vm2, T vm1, T vp1, T vp2, T vp3)
{
    const auto rec = WenoRecKernel(vm3, vm2, vm1, vp1, vp2, vp3);
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
