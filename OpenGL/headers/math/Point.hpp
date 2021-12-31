#ifndef POINT_HPP
# define POINT_HPP

// ************************************************************************** //
//                               Point Class                                  //
// ************************************************************************** //

# include "Fixed.hpp"

class Point
{
	public:
		Point();	// Default constructor
		Point(const Fixed, const Fixed, const Fixed);	// Constructor with two Fixed params
		Point(const Point &); // Copy constructor
		~Point();	// Destructor
		Point &operator=(const Point &);	// Copy-assignment operator
		Fixed get_x(void) const;	// Returns 'x'
		Fixed get_y(void) const;	// Returns 'y'
		Fixed get_z(void) const;	// Returns 'z'
	protected:
		Fixed	x;
		Fixed	y;
		Fixed	z;
};

#endif /* POINT_HPP */
