#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# define PRINT_HERE() (std::cerr << "File: " << __FILE__ << "Line: " << __LINE__ << std::endl)

# include <array>
# include "math/Matrix.hpp"
# include "Mesh.hpp"

namespace Utils
{
	template <typename T>
	T radians(const T& a)
	{
		return (a / static_cast<T>(180) * static_cast<T>(3.14159265358979323f));
	}

	std::array<Mesh::Vertex, 4> CreateQuad(float posX, float posY, float posZ, float size);
	std::array<Mesh::Vertex, 24> CreateCube(float posX, float posY, float posZ, float size);
	std::vector<unsigned int> CreateCube_Indices(void);
}

#endif
