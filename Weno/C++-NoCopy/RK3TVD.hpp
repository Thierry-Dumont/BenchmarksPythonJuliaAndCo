#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

/**
 * Explicit TVD Runge-Kutta method of order 3.
 */
// L: the RHS (du/dt =L(u)).
//
template <typename TRHS>
class RK3TVD
{
public:
    using RHS = typename std::decay<TRHS>::type;
    using Real = typename RHS::Real;

private:
    TRHS rhs;
    mutable std::vector<Real> u1, u2;

public:
    RK3TVD(TRHS rhs)
        : rhs(std::forward<TRHS>(rhs))
    {
        //std::cout << rhs.length << std::endl;
        //std::cout << this->rhs.length << std::endl;
    }

    /** Make one step.
     * @param[in,out]   data    initial value, overwritten.
     * @param           dt      time step.
     */
    template <typename TData>
    void step(TData & data, Real dt) const
    {
        std::size_t const size = data.size();
        u1.resize(size);
        u2.resize(size);

        // First step
        rhs(data, u1);
        for (std::size_t i = 0; i < size; ++i)
            u1[i] = data[i] + dt * u1[i];

        // Second step
        rhs(u1, u2);
        for (std::size_t i = 0; i < size; ++i)
            u2[i] = Real(3)/Real(4) * data[i] + Real(1)/Real(4) * (u1[i] + dt*u2[i]);

        // Third step
        rhs(u2, u1); // reuse u1.
        for (std::size_t i = 0; i < size; ++i)
            data[i] = Real(1)/Real(3) * data[i] + Real(2)/Real(3) * (u2[i] + dt*u1[i]);
    }
};

template <typename TRHS>
auto makeRK3TVD(TRHS && rhs)
{
    return RK3TVD<TRHS>(std::forward<TRHS>(rhs));
}
