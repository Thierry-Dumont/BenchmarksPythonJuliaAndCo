#pragma once
#include <utility>
#include <iostream>
//
// The Godunov flux.
//
template<class F> struct GodunovFlux{
  // constructors, for compatibility with other fluxes.
  F f;
  GodunovFlux(double *params=0){}
  GodunovFlux(GodunovFlux<F>& G)= delete;
  GodunovFlux& operator=(GodunovFlux<F>&& G){return *this;}
  inline double operator() (double a, double b)
  {
    return a<=b ?  f.min(a,b):f.max(b,a);
  }
  ~GodunovFlux()
  {
  }
};
