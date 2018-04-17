#pragma once
#include <algorithm>
struct  Convection
{
  //! the flux.
  static double flux(double x) 
  {
    return x;
  }
  //! min of flux  in [a,b]. Used by Godunov flux.
  //! we do NOT test a<=b !
  static double min(double a, double b)
  {

    if(b<=0)
      return flux(b);
    else if (a>=0)
      return flux(a);
    else
      return 0.0;
  }
  //! max of flux in [a,b]. Used by Godunov flux.
  //! we do NOT test a<=b !
  static double max(double a, double b)
  {
     return std::max(flux(a),flux(b));
  }
};
