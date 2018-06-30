#pragma once
#include <iostream>
#include <utility>
//
// The Lax-Friedrichs flux.
//
// alpha: the Lax-Friedrichs parameter.
template<class F> class LaxFriedrichsFlux{
  double alpha;
public:
  LaxFriedrichsFlux(){}
  LaxFriedrichsFlux(double *params):alpha(params[0]){}
  LaxFriedrichsFlux(LaxFriedrichsFlux<F>& L)=delete;
  LaxFriedrichsFlux& operator=(LaxFriedrichsFlux<F>&& L)
  {
    alpha=L.alpha;
    return *this;
  }
  double operator()  (double a, double b)
  {
     return 0.5*(F::flux(a)+F::flux(b) - alpha*(b-a));
  }
};
