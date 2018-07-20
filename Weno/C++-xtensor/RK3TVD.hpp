#pragma once
#include <memory>
#include <iostream>
#include "xtensor/xfixed.hpp"
#include "xtensor/xnoalias.hpp"
//
// Explicit TVD Runge-Kutta method of order 3.
//
// L: the RHS (du/dt =L(u)).
//
template<class L,std::size_t size> class RK3TVD
{
  xt::xtensor_fixed<double, xt::xshape<size>> u1,u2;
  const double c21=3./4.,c22=1./4.,c31=1./3.,c32=2./3.;

  L LL;
public:
  //! constructor.
  //\param size size of std::arrays.
  //\param Length length of the domain.
  RK3TVD(double Length,double params[])
  {

    LL=L(Length,params);
  }

  //! destructor
  ~RK3TVD()
  {
  }
  //! make one step.
  //! \param InOut intial value, overwritten..
  //! \param dt time step.
  void step(xt::xtensor_fixed<double, xt::xshape<size>>& InOut,double dt)
  {
    LL(InOut,u1);
    xt::noalias(u1)= InOut+dt*u1;

    LL(u1,u2);
    xt::noalias(u2)=c21*InOut+c22*(u1+dt*u2);

    LL(u2,u1); // reuse u1.
    xt::noalias(InOut)=c31*InOut+c32*(u2+dt*u1);
  }
};
