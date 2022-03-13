#ifndef QUATERNION_HPP
# define QUATERNION_HPP

// ************************************************************************** //
//                              Quaternion Class                              //
// ************************************************************************** //

# include <iostream>
# include <math.h>

template <typename T, unsigned int LENGTH>
class Vector;

template <typename T>
class Quaternion
{
    private:
        Vector<T, 4>                        data;
	public:
		Quaternion()                                        { }
		Quaternion(const Quaternion &q):    data(q.data)    { }
        template <typename... Args>
		Quaternion(const Args & ... args):  data(args...)   { }
		~Quaternion()                                       { }
		Quaternion &operator=(const Quaternion &q)          { if (this != &q) data = q.data; return (*this); }

		// Quaternion &unit_norm_rotate(void);	// normalize to its general rotational form

		Quaternion &operator+=(const Quaternion &q) { data[0] += q.data[0]; data[1] += q.data[1]; data[2] += q.data[2]; data[3] += q.data[3]; return (*this); }
		Quaternion &operator-=(const Quaternion &q) { data[0] -= q.data[0]; data[1] -= q.data[1]; data[2] -= q.data[2]; data[3] -= q.data[3]; return (*this); }
		Quaternion &operator*=(const T &a) { data[0] *= a; data[1] *= a; data[2] *= a; data[3] *= a; return (*this); }
		Quaternion &operator/=(const T &a) { data[0] /= a; data[1] /= a; data[2] /= a; data[3] /= a; return (*this); }

        T       &operator[](unsigned int index)       { return (data[index]); }
        T const &operator[](unsigned int index) const { return (data[index]); }
};

template <typename T>
Quaternion<T> conjugate(const Quaternion<T> &q) { return (Quaternion<T>(q[0], -q[1], -q[2], -q[3])); }

template <typename T>
T magnitude(const Quaternion<T> &q) { return (sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3])); }

template <typename T>
Quaternion<T> unit(const Quaternion<T> &q) { return (q / magnitude(q)); }

template <typename T>
Quaternion<T> inverse(const Quaternion<T> &q) { return (conjugate(q) / (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] * q[3] * q[3])); }

template <typename T>
std::ostream &operator<<(std::ostream &os, const Quaternion<T> &q)
{
    os << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3];
    return (os);
}

template <typename T>
Quaternion<T> operator+(const Quaternion<T> &q1, const Quaternion<T> &q2) { return (Quaternion<T>(q1[0] + q2[0], q1[1] + q2[1], q1[2] + q2[2], q1[3] + q2[3])); }

template <typename T>
Quaternion<T> operator-(const Quaternion<T> &q) { return (Quaternion<T> (-q[0], -q[1], -q[2], -q[3])); }

template <typename T>
Quaternion<T> operator-(const Quaternion<T> &q1, const Quaternion<T> &q2) { return (Quaternion<T>(q1[0] - q2[0], q1[1] - q2[1], q1[2] - q2[2], q1[3] - q2[3])); }

template <typename T>
Quaternion<T> operator*(const Quaternion<T> &q1, const Quaternion<T> &q2)
{
    return (Quaternion<T>(  q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3],
                            q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2],
                            q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1],
                            q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0]));
}

template <typename T>
Quaternion<T> operator*(const Quaternion<T> &q, const T &a) { return (Quaternion<T>(q[0] * a, q[1] * a, q[2] * a, q[3] * a)); }

template <typename T>
Quaternion<T> operator*(const T &a, const Quaternion<T> &q) { return (Quaternion<T>(q[0] * a, q[1] * a, q[2] * a, q[3] * a)); }

template <typename T>
Quaternion<T> operator/(const Quaternion<T> &q, const T &a) { return (Quaternion<T>(q[0] / a, q[1] / a, q[2] / a, q[3] / a)); }

#endif
