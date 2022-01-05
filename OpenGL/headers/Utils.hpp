#pragma once
#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# define PRINT_HERE() (std::cerr << "File: " << __FILE__ << "Line: " << __LINE__ << std::endl)

# include <array>

namespace Utils
{
	template <typename T>
	T radians(const T& a)
	{
		return (a / static_cast<T>(180) * static_cast<T>(3.14159265358979323f));
	}

	struct VertexTex
	{
		std::array<float, 3> position;
		std::array<float, 2> texCoords;
		float				 texId;
	};

	struct VertexColor
	{
		std::array<float, 3> position;
		std::array<float, 4> color;
	};

	typedef typename std::array<VertexTex, 4>					QuadTex;
	typedef typename std::array<std::array<VertexTex, 4>, 6>	CubeTex;

	std::array<VertexTex, 4> CreateQuad(float posX, float posY, float posZ, float size, float texId);
	std::array<std::array<VertexTex, 4>, 6> CreateCube(float posX, float posY, float posZ, float size, float texId);

	typedef typename std::array<VertexColor, 4>					QuadColor;
	typedef typename std::array<std::array<VertexColor, 4>, 6>	CubeColor;

	std::array<VertexColor, 4> CreateQuad(float posX, float posY, float posZ, float size, float red, float green, float blue, float alpha);
	std::array<std::array<VertexColor, 4>, 6> CreateCube(float posX, float posY, float posZ, float size, float red, float green, float blue, float alpha);
}

#endif
