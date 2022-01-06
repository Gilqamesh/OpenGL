#pragma once
#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# define PRINT_HERE() (std::cerr << "File: " << __FILE__ << "Line: " << __LINE__ << std::endl)

# include <array>
# include "math/Matrix.hpp"

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

	struct VertexTex_Normal
	{
		std::array<float, 3> position;
		std::array<float, 2> texCoords;
		float				 texId;
		std::array<float, 3> normal;
	};

	struct VertexColor
	{
		std::array<float, 3> position;
		std::array<float, 4> color;
	};

	struct VertexPosition
	{
		std::array<float, 3> position;
	};

	struct VertexPosition_Normal
	{
		std::array<float, 3> position;
		std::array<float, 3> normal;
	};

	struct VertexColor_Normal
	{
		std::array<float, 3> position;
		std::array<float, 4> color;
		std::array<float, 3> normal;
	};

	typedef typename std::array<VertexTex, 4>						QuadTex;
	typedef typename std::array<VertexTex_Normal, 4>				QuadTex_Normal;
	typedef typename std::array<VertexColor, 4>						QuadColor;
	typedef typename std::array<VertexColor_Normal, 4>				QuadColor_Normal;

	typedef typename std::array<QuadTex, 6>							CubeTex;
	typedef typename std::array<QuadTex_Normal, 6>					CubeTex_Normal;
	typedef typename std::array<QuadColor, 6>						CubeColor;
	typedef typename std::array<QuadColor_Normal, 6>				CubeColor_Normal;
	
	typedef typename std::array<VertexPosition, 4>					QuadPosition;
	typedef typename std::array<VertexPosition_Normal, 4>			QuadPosition_Normal;
	typedef typename std::array<QuadPosition, 6>					CubePosition;
	typedef typename std::array<QuadPosition_Normal, 6>				CubePosition_Normal;
	
	typedef typename std::array<unsigned int, 36>					CubeIndeces;

	template <typename T>
	T CreateQuad(float posX, float posY, float posZ, float size, float texId)
	{
		return {
			posX,		 posY, posZ,		0.0f, 0.0f, texId,
			posX + size, posY, posZ,		1.0f, 0.0f, texId,
			posX + size, posY, posZ + size, 1.0f, 1.0f, texId,
			posX	   , posY, posZ + size, 0.0f, 1.0f, texId
		};
	}
	template <typename T>
	T CreateQuad_Normal(float posX, float posY, float posZ, float size, float texId)
	{
		return {
			posX,		 posY, posZ,		0.0f, 0.0f, texId, 0.0f, 1.0f, 0.0f,
			posX + size, posY, posZ,		1.0f, 0.0f, texId, 0.0f, 1.0f, 0.0f,
			posX + size, posY, posZ + size, 1.0f, 1.0f, texId, 0.0f, 1.0f, 0.0f,
			posX	   , posY, posZ + size, 0.0f, 1.0f, texId, 0.0f, 1.0f, 0.0f
		};
	}
	template <typename T>
	T CreateQuad(float posX, float posY, float posZ, float size, float red, float green, float blue, float alpha)
	{
		return {
			posX,		 posY, posZ,		red, green, blue, alpha,
			posX + size, posY, posZ,		red, green, blue, alpha,
			posX + size, posY, posZ + size, red, green, blue, alpha,
			posX	   , posY, posZ + size, red, green, blue, alpha
		};
	}
	template <typename T>
	T CreateQuad_Normal(float posX, float posY, float posZ, float size, float red, float green, float blue, float alpha)
	{
		return {
			posX,		 posY, posZ,		red, green, blue, alpha, 0.0f, 1.0f, 0.0f,
			posX + size, posY, posZ,		red, green, blue, alpha, 0.0f, 1.0f, 0.0f,
			posX + size, posY, posZ + size, red, green, blue, alpha, 0.0f, 1.0f, 0.0f,
			posX	   , posY, posZ + size, red, green, blue, alpha, 0.0f, 1.0f, 0.0f
		};
	}
	
	template <typename T>
	T CreateCube(float posX, float posY, float posZ, float size, float texId)
	{
		return {
			posX,		 posY,		  posZ,        0.0f, 0.0f, texId,
			posX + size, posY,		  posZ,        1.0f, 0.0f, texId,
			posX + size, posY + size, posZ,        1.0f, 1.0f, texId,
			posX,	     posY + size, posZ,        0.0f, 1.0f, texId,

			posX,		 posY,		  posZ + size, 0.0f, 0.0f, texId,
			posX + size, posY,		  posZ + size, 1.0f, 0.0f, texId,
			posX + size, posY + size, posZ + size, 1.0f, 1.0f, texId,
			posX,		 posY + size, posZ + size, 0.0f, 1.0f, texId,

			posX,		 posY,		  posZ,		   0.0f, 0.0f, texId,
			posX,		 posY,		  posZ + size, 1.0f, 0.0f, texId,
			posX,		 posY + size, posZ + size, 1.0f, 1.0f, texId,
			posX,	     posY + size, posZ,		   0.0f, 1.0f, texId,

			posX + size, posY,		  posZ,		   0.0f, 0.0f, texId,
			posX + size, posY,		  posZ + size, 1.0f, 0.0f, texId,
			posX + size, posY + size, posZ + size, 1.0f, 1.0f, texId,
			posX + size, posY + size, posZ,		   0.0f, 1.0f, texId,

			posX,		 posY,		  posZ,		   0.0f, 0.0f, texId,
			posX + size, posY,		  posZ,		   1.0f, 0.0f, texId,
			posX + size, posY,		  posZ + size, 1.0f, 1.0f, texId,
			posX,		 posY,	      posZ + size, 0.0f, 1.0f, texId,

			posX,		 posY + size, posZ,		   0.0f, 0.0f, texId,
			posX + size, posY + size, posZ,		   1.0f, 0.0f, texId,
			posX + size, posY + size, posZ + size, 1.0f, 1.0f, texId,
			posX,		 posY + size, posZ + size, 0.0f, 1.0f, texId,
		};
	}
	template <typename T>
	T CreateCube_Normal(float posX, float posY, float posZ, float size, float texId)
	{
		return {
			posX,		 posY,		  posZ,        0.0f, 0.0f, texId,  0.0f,  0.0f, -1.0f,
			posX + size, posY,		  posZ,        1.0f, 0.0f, texId,  0.0f,  0.0f, -1.0f,
			posX + size, posY + size, posZ,        1.0f, 1.0f, texId,  0.0f,  0.0f, -1.0f,
			posX,	     posY + size, posZ,        0.0f, 1.0f, texId,  0.0f,  0.0f, -1.0f,
																	   	      
			posX,		 posY,		  posZ + size, 0.0f, 0.0f, texId,  0.0f,  0.0f,  1.0f,
			posX + size, posY,		  posZ + size, 1.0f, 0.0f, texId,  0.0f,  0.0f,  1.0f,
			posX + size, posY + size, posZ + size, 1.0f, 1.0f, texId,  0.0f,  0.0f,  1.0f,
			posX,		 posY + size, posZ + size, 0.0f, 1.0f, texId,  0.0f,  0.0f,  1.0f,
																	  	      
			posX,		 posY,		  posZ,		   0.0f, 0.0f, texId, -1.0f,  0.0f,  0.0f,
			posX,		 posY,		  posZ + size, 1.0f, 0.0f, texId, -1.0f,  0.0f,  0.0f,
			posX,		 posY + size, posZ + size, 1.0f, 1.0f, texId, -1.0f,  0.0f,  0.0f,
			posX,	     posY + size, posZ,		   0.0f, 1.0f, texId, -1.0f,  0.0f,  0.0f,
																	   
			posX + size, posY,		  posZ,		   0.0f, 0.0f, texId,  1.0f,  0.0f,  0.0f,
			posX + size, posY,		  posZ + size, 1.0f, 0.0f, texId,  1.0f,  0.0f,  0.0f,
			posX + size, posY + size, posZ + size, 1.0f, 1.0f, texId,  1.0f,  0.0f,  0.0f,
			posX + size, posY + size, posZ,		   0.0f, 1.0f, texId,  1.0f,  0.0f,  0.0f,
																	   
			posX,		 posY,		  posZ,		   0.0f, 0.0f, texId,  0.0f, -1.0f,  0.0f,
			posX + size, posY,		  posZ,		   1.0f, 0.0f, texId,  0.0f, -1.0f,  0.0f,
			posX + size, posY,		  posZ + size, 1.0f, 1.0f, texId,  0.0f, -1.0f,  0.0f,
			posX,		 posY,	      posZ + size, 0.0f, 1.0f, texId,  0.0f, -1.0f,  0.0f,
																	   
			posX,		 posY + size, posZ,		   0.0f, 0.0f, texId,  0.0f,  1.0f,  0.0f,
			posX + size, posY + size, posZ,		   1.0f, 0.0f, texId,  0.0f,  1.0f,  0.0f,
			posX + size, posY + size, posZ + size, 1.0f, 1.0f, texId,  0.0f,  1.0f,  0.0f,
			posX,		 posY + size, posZ + size, 0.0f, 1.0f, texId,  0.0f,  1.0f,  0.0f
		};
	}
	template <typename T>
	T CreateCube(float posX, float posY, float posZ, float size)
	{
		return {
			posX,		 posY,		  posZ,
			posX + size, posY,		  posZ,
			posX + size, posY + size, posZ,
			posX,	     posY + size, posZ,

			posX,		 posY,		  posZ + size,
			posX + size, posY,		  posZ + size,
			posX + size, posY + size, posZ + size,
			posX,		 posY + size, posZ + size,

			posX,		 posY,		  posZ,
			posX,		 posY,		  posZ + size,
			posX,		 posY + size, posZ + size,
			posX,	     posY + size, posZ,

			posX + size, posY,		  posZ,
			posX + size, posY,		  posZ + size,
			posX + size, posY + size, posZ + size,
			posX + size, posY + size, posZ,

			posX,		 posY,		  posZ,
			posX + size, posY,		  posZ,
			posX + size, posY,		  posZ + size,
			posX,		 posY,	      posZ + size,

			posX,		 posY + size, posZ,
			posX + size, posY + size, posZ,
			posX + size, posY + size, posZ + size,
			posX,		 posY + size, posZ + size
		};
	}
	template <typename T>
	T CreateCube_Normal(float posX, float posY, float posZ, float size)
	{
		return {
			posX,		 posY,		  posZ,		    0.0f,  0.0f, -1.0f,
			posX + size, posY,		  posZ,		    0.0f,  0.0f, -1.0f,
			posX + size, posY + size, posZ,		    0.0f,  0.0f, -1.0f,
			posX,	     posY + size, posZ,		    0.0f,  0.0f, -1.0f,
												    
			posX,		 posY,		  posZ + size,  0.0f,  0.0f,  1.0f,
			posX + size, posY,		  posZ + size,  0.0f,  0.0f,  1.0f,
			posX + size, posY + size, posZ + size,  0.0f,  0.0f,  1.0f,
			posX,		 posY + size, posZ + size,  0.0f,  0.0f,  1.0f,

			posX,		 posY,		  posZ,		   -1.0f,  0.0f,  0.0f,
			posX,		 posY,		  posZ + size, -1.0f,  0.0f,  0.0f,
			posX,		 posY + size, posZ + size, -1.0f,  0.0f,  0.0f,
			posX,	     posY + size, posZ,		   -1.0f,  0.0f,  0.0f,

			posX + size, posY,		  posZ,		    1.0f,  0.0f,  0.0f,
			posX + size, posY,		  posZ + size,  1.0f,  0.0f,  0.0f,
			posX + size, posY + size, posZ + size,  1.0f,  0.0f,  0.0f,
			posX + size, posY + size, posZ,		    1.0f,  0.0f,  0.0f,
												    
			posX,		 posY,		  posZ,		    0.0f, -1.0f,  0.0f,
			posX + size, posY,		  posZ,		    0.0f, -1.0f,  0.0f,
			posX + size, posY,		  posZ + size,  0.0f, -1.0f,  0.0f,
			posX,		 posY,	      posZ + size,  0.0f, -1.0f,  0.0f,
												    
			posX,		 posY + size, posZ,		    0.0f,  1.0f,  0.0f,
			posX + size, posY + size, posZ,		    0.0f,  1.0f,  0.0f,
			posX + size, posY + size, posZ + size,  0.0f,  1.0f,  0.0f,
			posX,		 posY + size, posZ + size,  0.0f,  1.0f,  0.0f
		};
	}

	CubeIndeces CreateCube_Indices(void);
}

#endif
