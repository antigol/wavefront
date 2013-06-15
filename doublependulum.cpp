#include "doublependulum.h"
#include <cmath>
#include <complex>
#include <QDebug>

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

        //
        // u''(t) = f(u(t), u'(t))
        // anciene magniere : u(t+1) = u(t) + u'(t) * dt + 1/2 * u''(t) * dt^2
        // nouvelle : Runge-Kutta
        double c1=-(_l2*(g*(-6*cosa1*_m2-3*cosa1*_m1)+9*cosa2a1*sina2a1*_b1*_b1*_l1*_m2)
                    +6*sina2a1*_b2*_b2*_l2*_l2*_m2+9*cosa2*cosa2a1*g*_m2)
                /(_l1*_l2*(9*cosa2a1*cosa2a1*_m2-12*_m2-4*_m1));

        double c2=(_l2*(cosa2a1*g*(-18*cosa1*_m2-9*cosa1*_m1)+sina2a1*_b1*_b1*_l1*(36*_m2+12*_m1))
                   +cosa2*g*(36*_m2+12*_m1)+18*cosa2a1*sina2a1*_b2*_b2*_l2*_l2*_m2)
                /(_l2*_l2*(18*cosa2a1*cosa2a1*_m2-24*_m2-8*_m1));

        _a1 += _b1 * ddt + 0.5 * c1 * ddt*ddt;
        _a2 += _b2 * ddt + 0.5 * c2 * ddt*ddt;
        _b1 += c1 * ddt;
        _b2 += c2 * ddt;
    }
}

void DoublePendulum::move2(double dt)
{
    const double h = dt / double(_n/10);
    Vec4 x(_a1, _a2, _b1, _b2);
    for (int i = 0; i < _n/10; ++i) {
        Vec4 p1 = function(x);
        Vec4 p2 = function(x + p1 * (h / 2.0));
        Vec4 p3 = function(x + p2 * (h / 2.0));
        Vec4 p4 = function(x + p3 * dt);
        x += (h / 6.0) * (p1 + 2.0 * p2 + 2.0 * p3 + p4);
    }
    _a1 = x.x();
    _a2 = x.y();
    _b1 = x.z();
    _b2 = x.w();
}

Vec4 DoublePendulum::function(const Vec4 &x)
{
    const double g = 9.81;
    double cosa2a1 = cos(x.y() - x.x());
    double sina2a1 = sin(x.y() - x.x());
    double cosa1 = cos(x.x());
    double cosa2 = cos(x.y());

    Vec4 r;
    r.x(x.z());
    r.y(x.w());
    r.z(-(_l2*(g*(-6*cosa1*_m2-3*cosa1*_m1)+9*cosa2a1*sina2a1*x.z()*x.z()*_l1*_m2)
          +6*sina2a1*x.w()*x.w()*_l2*_l2*_m2+9*cosa2*cosa2a1*g*_m2)
        /(_l1*_l2*(9*cosa2a1*cosa2a1*_m2-12*_m2-4*_m1)));
    r.w((_l2*(cosa2a1*g*(-18*cosa1*_m2-9*cosa1*_m1)+sina2a1*x.z()*x.z()*_l1*(36*_m2+12*_m1))
         +cosa2*g*(36*_m2+12*_m1)+18*cosa2a1*sina2a1*x.w()*x.w()*_l2*_l2*_m2)
        /(_l2*_l2*(18*cosa2a1*cosa2a1*_m2-24*_m2-8*_m1)));
    
    return r;
}

double DoublePendulum::kinetic() const
{
    double cin = 2.0/3.0 * _b1*_b1 * _m1 * _l1*_l1
            + 2.0 * _b1*_b1 * _m2 * _l1*_l1
            + 2.0 * cos(_a1-_a2) * _b1*_b2 * _m2 * _l1*_l2
            + 2.0/3.0 * _b2*_b2 * _m2 * _l2*_l2;
    return cin;
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
