#pragma once
#include <algorithm>
#include <string>
struct  Burghers
{
  //! the flux.
  static double flux(double x) 
  {
    return 0.5*x*x;
  }
  //! min of flux  in [a,b]. Used by Godunov flux.
  //! we do NOT test a<=b !
  static double min(double a, double b)
  {

    if(b<=0)
      return  0.5*b*b;
    else if (a>=0)
      return  0.5*a*a;
    else
      return 0.0;
  }
  //! max of flux in [a,b]. Used by Godunov flux.
  //! we do NOT test a<=b !
  static double max(double a, double b)
  {
     return 0.5*std::max(a*a,b*b);
  }
};
