#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H

class Vec4;

class DoublePendulum
{
public:
    DoublePendulum(int n = 1000);

    void setParameters(double m1, double m2, double l1, double l2, double a1, double a2);
    void reset();

    void move(double dt);
    void move2(double dt);
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
    Vec4 function(const Vec4& x);
    double _a01, _a02;

    double _a1, _b1;
    double _a2, _b2;

    double _m1;
    double _m2;
    double _l1;
    double _l2;

    int _n;
    double _dt;
};

class Vec4 {
public:
    //Constructeurs (dont par défaut, et de copie)
    Vec4(double x, double y, double z, double w);
    Vec4();
    Vec4(const Vec4& other);

    //Méthodes "get"
    inline double x() const { return _x; }
    inline double y() const { return _y; }
    inline double z() const { return _z; }
    inline double w() const { return _w; }

    //Méthodes "set"
    inline double x(double __x) { _x = __x; return __x; }
    inline double y(double __y) { _y = __y; return __y; }
    inline double z(double __z) { _z = __z; return __z; }
    inline double w(double __w) { _w = __w; return __w; }

    //Opérateurs de comparaison et d'affectation
    bool operator==(const Vec4& other) const;
    bool operator!=(const Vec4& other) const;

    //Opérateurs mathématiques
    Vec4& operator+=(const Vec4& other);
    friend Vec4 operator+(Vec4 vect1, const Vec4& vect2);
    Vec4 operator-() const;
    Vec4& operator-=(const Vec4& other);
    friend Vec4 operator-(Vec4 vect1, const Vec4& vect2);
    Vec4& operator*=(double nb);
    friend Vec4 operator*(Vec4 vect, double nb);
    friend Vec4 operator*(double nb, Vec4 vect);
    double operator*(const Vec4& other) const;

private:
    //Attributs : composantes cartésiennes
    double _x;
    double _y;
    double _z;
    double _w;
};

inline Vec4::Vec4(double x, double y, double z, double w) : _x(x), _y(y), _z(z), _w(w) {}
inline Vec4::Vec4() : Vec4(0.0,0.0,0.0,0.0) {}
inline Vec4::Vec4(const Vec4& other) : Vec4(other._x, other._y, other._z, other._w) {}

inline bool Vec4::operator==(const Vec4& other) const { return _x == other._x && _y == other._y && _z == other._y && _w == other._w; }
inline bool Vec4::operator!=(const Vec4& other) const { return !(*this == other); }

inline Vec4& Vec4::operator+=(const Vec4& other) {
    _x += other._x;
    _y += other._y;
    _z += other._z;
    _w += other._w;
    return *this;
}
inline Vec4 operator+(Vec4 vect1, const Vec4& vect2) { return vect1 += vect2; }
inline Vec4 Vec4::operator-() const { return Vec4(-_x,-_y,-_z,-_w); }
inline Vec4& Vec4::operator-=(const Vec4& other) {
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    _w -= other._w;
    return *this;
}
inline Vec4 operator-(Vec4 vect1, const Vec4& vect2) { return vect1 -= vect2; }
inline Vec4& Vec4::operator*=(double nb) {
    _x *= nb;
    _y *= nb;
    _z *= nb;
    _w *= nb;
    return *this;
}
inline Vec4 operator*(Vec4 vect, double nb) {
    return vect*= nb;
}
inline Vec4 operator*(double nb, Vec4 vect) {
    return vect * nb;
}
inline double Vec4::operator*(const Vec4& other) const {
    return _x*other._x + _y*other._y + _z*other._z + _w*other._w;
}

#endif // DOUBLEPENDULUM_H
