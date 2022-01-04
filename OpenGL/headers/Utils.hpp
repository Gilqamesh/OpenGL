#pragma once
#ifndef UTILS_HPP
# define UTILS_HPP

namespace Utils
{
	template <typename T>
	T radians(const T &a)
	{
		return (a / static_cast<T>(180) * static_cast<T>(3.14159265358979323f));
	}
}

#endif
