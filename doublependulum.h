#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H

class DoublePendulum
{
public:
    DoublePendulum(int n = 1000);

    void setParameters(double m1, double m2, double l1, double l2, double a1, double a2);
    void reset();

    void move(double dt);
    double kinetic() const;
    double potential() const;

    double a1() const;
    double a2() const;
    double b1() const;
    double b2() const;
    double l1() const;
    double l2() const;
    double m1() const;
    double m2() const;

private:
    double _a01, _a02;

    double _a1, _b1;
    double _a2, _b2;

    double _m1;
    double _m2;
    double _l1;
    double _l2;

    int _n;
};

#endif // DOUBLEPENDULUM_H
