#pragma once

#include <utility>
#include <type_traits>

/**
 * The Lax-Friedrichs flux.
 *
 * @tparam  TProblem    Type of the problem
 */
template <typename TProblem>
class LaxFriedrichsFlux
{
public:
    using Problem = typename std::decay<TProblem>::type;
    using Real = typename Problem::Real;

private:
    TProblem f;
    Real alpha;

public:
    /**
     * @param f     The problem
     * @param alpha The Lax-Friedrichs parameter.
     */
    LaxFriedrichsFlux(TProblem f, Real alpha = 1.)
        : f(std::forward<TProblem>(f)), alpha(alpha)
    {}

    double operator() (double a, double b) const
    {
        return 0.5 * (f.flux(a) + f.flux(b) - alpha * (b - a));
    }
};

template <typename TProblem, typename TReal>
auto makeLaxFriedrichsFlux(TProblem && problem, TReal const& alpha)
{
    return LaxFriedrichsFlux<TProblem>(std::forward<TProblem>(problem), alpha);
}
