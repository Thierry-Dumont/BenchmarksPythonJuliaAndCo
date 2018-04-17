#pragma once
#include <utility>
//
// The Godunov flux.
//
template<class F> struct GodunovFlux{
  // constructors, for compatibility with other fluxes.
  GodunovFlux(double *params=0){}
  GodunovFlux(GodunovFlux<F>& G)= delete;
  GodunovFlux& operator=(GodunovFlux<F>&& G){return *this;}
  double operator() (double a, double b)
  {
    if(a<=b)
      return F::min(a,b);
    else
      return F::max(b,a);
  }
};
