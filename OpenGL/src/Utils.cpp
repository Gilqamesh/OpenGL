#include "Utils.hpp"

namespace Utils
{
	std::array<VertexTex, 4> CreateQuad(float posX, float posY, float posZ, float size, float texId)
	{
		return {
			posX,		 posY, posZ,		0.0f, 0.0f, texId,
			posX + size, posY, posZ,		1.0f, 0.0f, texId,
			posX + size, posY, posZ + size, 1.0f, 1.0f, texId,
			posX	   , posY, posZ + size, 0.0f, 1.0f, texId
		};
	}

	std::array<std::array<VertexTex, 4>, 6> CreateCube(float posX, float posY, float posZ, float size, float texId)
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

	std::array<VertexColor, 4> CreateQuad(float posX, float posY, float posZ, float size, float red, float green, float blue, float alpha)
	{
		return {
			posX,		 posY, posZ,		red, green, blue, alpha,
			posX + size, posY, posZ,		red, green, blue, alpha,
			posX + size, posY, posZ + size, red, green, blue, alpha,
			posX	   , posY, posZ + size, red, green, blue, alpha
		};
	}

	std::array<std::array<VertexColor, 4>, 6> CreateCube(float posX, float posY, float posZ, float size, float red, float green, float blue, float alpha)
	{
		return {
			posX,		 posY,		  posZ,        red, green, blue, alpha,
			posX + size, posY,		  posZ,        red, green, blue, alpha,
			posX + size, posY + size, posZ,        red, green, blue, alpha,
			posX,	     posY + size, posZ,        red, green, blue, alpha,
													
			posX,		 posY,		  posZ + size, red, green, blue, alpha,
			posX + size, posY,		  posZ + size, red, green, blue, alpha,
			posX + size, posY + size, posZ + size, red, green, blue, alpha,
			posX,		 posY + size, posZ + size, red, green, blue, alpha,
													
			posX,		 posY,		  posZ,		   red, green, blue, alpha,
			posX,		 posY,		  posZ + size, red, green, blue, alpha,
			posX,		 posY + size, posZ + size, red, green, blue, alpha,
			posX,	     posY + size, posZ,		   red, green, blue, alpha,
													
			posX + size, posY,		  posZ,		   red, green, blue, alpha,
			posX + size, posY,		  posZ + size, red, green, blue, alpha,
			posX + size, posY + size, posZ + size, red, green, blue, alpha,
			posX + size, posY + size, posZ,		   red, green, blue, alpha,
													
			posX,		 posY,		  posZ,		   red, green, blue, alpha,
			posX + size, posY,		  posZ,		   red, green, blue, alpha,
			posX + size, posY,		  posZ + size, red, green, blue, alpha,
			posX,		 posY,	      posZ + size, red, green, blue, alpha,
													
			posX,		 posY + size, posZ,		   red, green, blue, alpha,
			posX + size, posY + size, posZ,		   red, green, blue, alpha,
			posX + size, posY + size, posZ + size, red, green, blue, alpha,
			posX,		 posY + size, posZ + size, red, green, blue, alpha
		};
	}
}
