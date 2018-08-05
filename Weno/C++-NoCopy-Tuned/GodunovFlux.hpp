#pragma once

#include <utility>
#include <type_traits>

/**
 * The Godunov flux.
 *
 * @tparam  TProblem    Type of the problem
 */
template <typename TProblem>
class GodunovFlux
{
public:
    using Problem = typename std::decay<TProblem>::type;
    using Real = typename Problem::Real;

private:
    TProblem f;

public:

  /**
   * @param f   The problem.
   */
  GodunovFlux(TProblem f)
      : f(std::forward<TProblem>(f))
  {}

  inline Real operator() (Real a, Real b) const
  {
#ifdef USE_ORIGINAL_VERSION
    return a <= b ?  f.min(a, b) : f.max(b, a); // Original version
#else // Tuned version
    const Real fmin = f.min(a, b);
    const Real fmax = f.max(b, a);
    return a <= b ? fmin : fmax;
#endif
  }
};

template <typename TProblem>
GodunovFlux<TProblem> makeGodunovFlux(TProblem && problem)
{
    return {std::forward<TProblem>(problem)};
}
