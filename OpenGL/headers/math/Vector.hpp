#ifndef VECTOR_HPP
# define VECTOR_HPP

// ************************************************************************** //
//                              Vector Class                                  //
// ************************************************************************** //

# include "Quaternion.hpp"
# include <math.h>

template <typename T, unsigned int ROWS, unsigned int COLUMNS>
class Matrix;

template <typename T, unsigned int LENGTH>
class Vector;

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> cross_product(const Vector<T, LENGTH> &v1, const Vector<T, LENGTH> &v2);

template <typename T, unsigned int LENGTH>
class Vector
{
    friend Vector cross_product<T, LENGTH>(const Vector &v1, const Vector &v2);
    private:
        Matrix<T, 1, LENGTH>                    data;
	public:
		Vector()                                                { }
        Vector(const Matrix<T, 1, LENGTH> &m):  data(m)         { }
        template <typename... Args>
        Vector(const Args & ... args):          data(args...)   { }
		Vector(const Vector &v):                data(v.data)    { }
		~Vector()                                               { }
		Vector &operator=(const Vector &v)                      { if (this != &v) data = v.data; return (*this); }

		Vector &operator+=(const Vector &v) { for (unsigned int i = 0; i < LENGTH; ++i) (*this)[i] = (*this)[i] + v[i]; return (*this); }
		Vector &operator-=(const Vector &v) { for (unsigned int i = 0; i < LENGTH; ++i) (*this)[i] = (*this)[i] - v[i]; return (*this); }
		Vector &operator*=(const T &a)      { for (unsigned int i = 0; i < LENGTH; ++i) (*this)[i] = (*this)[i] * a; return (*this); }
		Vector &operator/=(const T &a)      { for (unsigned int i = 0; i < LENGTH; ++i) (*this)[i] = (*this)[i] / a; return (*this); }

        T       &operator[](unsigned int index)       { return (data(0, index)); }
        T const &operator[](unsigned int index) const { return (data(0, index)); }

        T       *operator&()       { return (&data); }
        T const *operator&() const { return (&data); }
};

template <typename T, unsigned int LENGTH>
std::ostream &operator<<(std::ostream &os, const Vector<T, LENGTH> &v)
{
    for (unsigned int i = 0; i < LENGTH; ++i)
    {
        os << v[i];
        if (i != LENGTH - 1)
            os << ", ";
    }
    return (os);
}

template <typename T, unsigned int LENGTH>
T magnitude(const Vector<T, LENGTH> &v)
{
    T   r(0);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r += v[i] * v[i];
    return (sqrt(r));
}

// returns scalar of projecting vector v1 onto v2
template <typename T, unsigned int LENGTH>
T dot_product(const Vector<T, LENGTH> &v1, const Vector<T, LENGTH> &v2)
{
    T   r(0);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r += v1[i] * v2[i];
    return (r);
}

template <typename T, unsigned int LENGTH>
T angle(const Vector<T, LENGTH> &v1, const Vector<T, LENGTH> &v2) { return (dot_product(v1, v2) / magnitude(v1) / magnitude(v2)); }

template <typename T>
Vector<T, 3> cross_product(const Vector<T, 3> &v1, const Vector<T, 3> &v2) {
    return (Vector<T, 3>(v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0]));
}

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> unit(const Vector<T, LENGTH> &v) { return (v / magnitude(v)); }

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> normalize(const Vector<T, LENGTH> &v) { return (v / magnitude(v)); }

template <typename T>
Vector<T, 3> rotate(const Vector<T, 3> &v, const T &angle, const Vector<T, 3> &axis)
{
    Vector<T, 3>    e(unit(axis));

    // Rodrigues' rotation formula
    return (Vector<T, 3>(cos(angle) * v + sin(angle) * cross_product(e, v) + (1 - cos(angle)) * dot_product(e, v) * e));
}

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> operator/(const Vector<T, LENGTH> &v, const T &a)
{
    Vector<T, LENGTH>   r(v);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r[i] = r[i] / a;
    return (r);
}

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> operator+(const Vector<T, LENGTH> &v1, const Vector<T, LENGTH> &v2)
{
    Vector<T, LENGTH>   r(v1);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r[i] = r[i] + v2[i];
    return (r);
}

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> operator-(const Vector<T, LENGTH> &v)
{
    Vector<T, LENGTH>   r(v);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r[i] = -r[i];
    return (r);
}

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> operator-(const Vector<T, LENGTH> &v1, const Vector<T, LENGTH> &v2)
{
    Vector<T, LENGTH>   r(v1);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r[i] = r[i] - v2[i];
    return (r);
}

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> operator*(const Vector<T, LENGTH> &v, const T &a)
{
    Vector<T, LENGTH>   r(v);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r[i] = r[i] * a;
    return (r);
}

template <typename T, unsigned int LENGTH>
Vector<T, LENGTH> operator*(const T &a, const Vector<T, LENGTH> &v)
{
    Vector<T, LENGTH>   r(v);

    for (unsigned int i = 0; i < LENGTH; ++i)
        r[i] = r[i] * a;
    return (r);
}

template <typename T, unsigned int LENGTH>
bool operator==(const Vector<T, LENGTH> &v1, const Vector<T, LENGTH> &v2)
{
    for (unsigned int i = 0; i < LENGTH; ++i)
        if (v1[i] != v2[i])
            return (false);
    return (true);
}

template <typename T, unsigned int LENGTH>
bool operator!=(const Vector<T, LENGTH> &v1, const Vector<T, LENGTH> &v2)
{
    for (unsigned int i = 0; i < LENGTH; ++i)
        if (v1[i] != v2[i])
            return (true);
    return (false);
}

#endif
