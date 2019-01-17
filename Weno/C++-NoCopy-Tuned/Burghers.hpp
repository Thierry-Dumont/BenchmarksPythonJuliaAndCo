#pragma once

#include <algorithm>

template <typename TReal = double>
struct  Burghers
{
    using Real = TReal;

    //! the flux.
    static inline
    Real flux(Real x)
    {
        return 0.5 * x * x;
    }

    //! min of flux  in [a,b]. Used by Godunov flux.
    //! we do NOT test a<=b !
    static inline
    Real min(Real a, Real b)
    {
        if (b <= 0)
            return flux(b);
        else if (a >= 0)
            return flux(a);
        else
            return Real(0);
    }

    //! max of flux in [a,b]. Used by Godunov flux.
    //! we do NOT test a<=b !
    static inline
    Real max(Real a, Real b)
    {
        return std::max(flux(a), flux(b));
    }
};
