#include "doublependulum.h"
#include <cmath>
#include <complex>

DoublePendulum::DoublePendulum(int n) :
    _n(n)
{
}

void DoublePendulum::setParameters(double m1, double m2, double l1, double l2, double a1, double a2)
{
    _m1 = m1;
    _m2 = m2;
    _l1 = l1;
    _l2 = l2;
    _a01 = a1;
    _a02 = a2;
}

void DoublePendulum::reset()
{
    _a1 = _a01 * M_PI / 180.0;
    _a2 = _a02 * M_PI / 180.0;
    _b1 = 0.0;
    _b2 = 0.0;
}

void DoublePendulum::move(double dt)
{
    const double g = 9.81;
    const double ddt = dt / double(_n);

    for (int i = 0; i < _n; ++i) {
        double cosa2a1 = cos(_a2-_a1);
        double sina2a1 = sin(_a2-_a1);
        double cosa1 = cos(_a1);
        double cosa2 = cos(_a2);

        double c1=-(_l2*(g*(-6*cosa1*_m2-3*cosa1*_m1)+9*cosa2a1*sina2a1*_b1*_b1*_l1*_m2)
                    +6*sina2a1*_b2*_b2*_l2*_l2*_m2+9*cosa2*cosa2a1*g*_m2)
                /(_l1*_l2*(9*cosa2a1*cosa2a1*_m2-12*_m2-4*_m1));

        double c2=(_l2*(cosa2a1*g*(-18*cosa1*_m2-9*cosa1*_m1)+sina2a1*_b1*_b1*_l1*(36*_m2+12*_m1))
                   +cosa2*g*(36*_m2+12*_m1)+18*cosa2a1*sina2a1*_b2*_b2*_l2*_l2*_m2)
                /(_l2*_l2*(18*cosa2a1*cosa2a1*_m2-24*_m2-8*_m1));

        _b1 += c1 * ddt;
        _b2 += c2 * ddt;
        _a1 += _b1 * ddt;
        _a2 += _b2 * ddt;
    }
}

double DoublePendulum::kinetic() const
{
    double r1 = 1.0 / 6.0 * _m1 * _l1*_l1 * _b1*_b1;
    double r2 = 1.0 / 6.0 * _m2 * _l2*_l2 * _b2*_b2;
    double c1 = 1.0 / 2.0 * _m1 * _l1*_l1 * _b1*_b1;
    std::complex<double> i(0, 1);
    std::complex<double> s2 = i*(2.0*_b1*_l1 * std::exp(i*_a1) + _b2*_l2 * std::exp(i*_a2));
    double c2 = 1.0 / 2.0 * _m2 * std::norm(s2);

    return r1 + r2 + c1 + c2;
}

double DoublePendulum::potential() const
{
    const double g = 9.81;
    double h1 = sin(_a1) * _l1;
    return g * (_m1 * h1 + _m2 * (2.0 * h1 + sin(_a2) * _l2));
}

double DoublePendulum::a1() const
{
    return _a1 * 180.0 / M_PI;
}

double DoublePendulum::a2() const
{
    return _a2 * 180.0 / M_PI;
}

double DoublePendulum::b1() const
{
    return _b1 * 180.0 / M_PI;
}

double DoublePendulum::b2() const
{
    return _b2 * 180.0 / M_PI;
}

double DoublePendulum::l1() const
{
    return _l1;
}

double DoublePendulum::l2() const
{
    return _l2;
}

double DoublePendulum::m1() const
{
    return _m1;
}

double DoublePendulum::m2() const
{
    return _m2;
}
