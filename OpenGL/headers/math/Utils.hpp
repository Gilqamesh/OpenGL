#ifndef UTILS_HPP
# define UTILS_HPP

# include "Point.hpp"

namespace Utils
{
	extern const Fixed	PI;
	// 'a', 'b' and 'c' are vertices to a triangle
	// Returns true if 'point' is inside the triangle, false if 'point' is outside or
	// part of the perimeter of the triangle.
	bool				bsp(Point const &a, Point const &b, Point const &c, Point const &point);
	Fixed				degree_to_rad(const Fixed &);
	Fixed				rad_to_degree(const Fixed &);
}	// Utils

#endif /* UTILS_HPP */
