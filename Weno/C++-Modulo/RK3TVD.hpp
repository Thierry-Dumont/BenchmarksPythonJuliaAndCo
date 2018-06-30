#pragma once
#include <memory>
#include <iostream>
//
// Explicit TVD Runge-Kutta method of order 3.
//
// L: the RHS (du/dt =L(u)).
//
template<class L> class RK3TVD
{
  const int size;
  std::unique_ptr<double[]> u1,u2; 
  const double c21=3./4.,c22=1./4.,c31=1./3.,c32=2./3.;

  L LL;
public:
  //! constructor.
  //\param size size of arrays.
  //\param Length length of the domain.
  RK3TVD(int _size, double Length,double params[]):size(_size)
  {
    u1=std::make_unique<double[]>(size);
    u2=std::make_unique<double[]>(size);
    LL=L(size,Length,params);
  }

  //! destructor
  ~RK3TVD()
  {
  }
  //! make one step.
  //! \param InOut intial value, overwritten..
  //! \param dt time step.
  void step(std::unique_ptr<double []>& InOut,double dt)
  {
    LL(InOut,u1);
    for(int i=0;i<size;i++)
      u1[i]=InOut[i] + dt*u1[i];

    LL(u1,u2);
    for(int i=0;i<size;i++)
      u2[i]= c21*InOut[i]+c22*(u1[i]+dt*u2[i]);

    LL(u2,u1); // reuse u1.
    for(int i=0;i<size;i++)
      InOut[i]=c31*InOut[i]+c32*(u2[i]+dt*u1[i]);
  }
};
