#include "Utils.hpp"

namespace Utils
{
	std::array<Mesh::Vertex, 4> CreateQuad(float posX, float posY, float posZ, float size)
	{
		return {
			Vector<float, 3>(posX, posY, posZ),					Vector<float, 3>(0.0f, 1.0f, 0.0f), Vector<float, 2>(0.0f, 0.0f),
			Vector<float, 3>(posX + size, posY, posZ),			Vector<float, 3>(0.0f, 1.0f, 0.0f), Vector<float, 2>(1.0f, 0.0f),
			Vector<float, 3>(posX + size, posY, posZ + size),	Vector<float, 3>(0.0f, 1.0f, 0.0f), Vector<float, 2>(1.0f, 1.0f),
			Vector<float, 3>(posX, posY, posZ + size),			Vector<float, 3>(0.0f, 1.0f, 0.0f), Vector<float, 2>(0.0f, 1.0f)
		};
	}
	std::array<Mesh::Vertex, 24> CreateCube(float posX, float posY, float posZ, float size)
	{
		return {
			Vector<float, 3>(posX,		 posY,		  posZ),		 Vector<float, 3>(0.0f, 0.0f, -1.0f), Vector<float, 2>(0.0f,  0.0f),
			Vector<float, 3>(posX + size, posY,		  posZ),		 Vector<float, 3>(0.0f, 0.0f, -1.0f), Vector<float, 2>(1.0f,  0.0f),
			Vector<float, 3>(posX + size, posY + size, posZ),		 Vector<float, 3>(0.0f, 0.0f, -1.0f), Vector<float, 2>(1.0f,  1.0f),
			Vector<float, 3>(posX,	     posY + size, posZ),		 Vector<float, 3>(0.0f, 0.0f, -1.0f), Vector<float, 2>(0.0f,  1.0f),

			Vector<float, 3>(posX,		 posY,		  posZ + size),  Vector<float, 3>(0.0f, 0.0f, 1.0f)	, Vector<float, 2>(0.0f,  0.0f),
			Vector<float, 3>(posX + size, posY,		  posZ + size),  Vector<float, 3>(0.0f, 0.0f, 1.0f)	, Vector<float, 2>(1.0f,  0.0f),
			Vector<float, 3>(posX + size, posY + size, posZ + size), Vector<float, 3>(0.0f, 0.0f, 1.0f)	, Vector<float, 2>(1.0f,  1.0f),
			Vector<float, 3>(posX,		 posY + size, posZ + size),  Vector<float, 3>(0.0f, 0.0f, 1.0f)	, Vector<float, 2>(0.0f,  1.0f),

			Vector<float, 3>(posX,		 posY,		  posZ),		 Vector<float, 3>(-1.0f, 0.0f, 0.0f), Vector<float, 2>(0.0f,  0.0f),
			Vector<float, 3>(posX,		 posY,		  posZ + size),  Vector<float, 3>(-1.0f, 0.0f, 0.0f), Vector<float, 2>(1.0f,  0.0f),
			Vector<float, 3>(posX,		 posY + size, posZ + size),  Vector<float, 3>(-1.0f, 0.0f, 0.0f), Vector<float, 2>(1.0f,  1.0f),
			Vector<float, 3>(posX,	     posY + size, posZ),		 Vector<float, 3>(-1.0f, 0.0f, 0.0f), Vector<float, 2>(0.0f,  1.0f),

			Vector<float, 3>(posX + size, posY,		  posZ),		 Vector<float, 3>(1.0f, 0.0f, 0.0f)	, Vector<float, 2>(0.0f,  0.0f),
			Vector<float, 3>(posX + size, posY,		  posZ + size),  Vector<float, 3>(1.0f, 0.0f, 0.0f)	, Vector<float, 2>(1.0f,  0.0f),
			Vector<float, 3>(posX + size, posY + size, posZ + size), Vector<float, 3>(1.0f, 0.0f, 0.0f)	, Vector<float, 2>(1.0f,  1.0f),
			Vector<float, 3>(posX + size, posY + size, posZ),		 Vector<float, 3>(1.0f, 0.0f, 0.0f)	, Vector<float, 2>(0.0f,  1.0f),

			Vector<float, 3>(posX,		 posY,		  posZ),		 Vector<float, 3>(0.0f, -1.0f, 0.0f), Vector<float, 2>(0.0f,  0.0f),
			Vector<float, 3>(posX + size, posY,		  posZ),		 Vector<float, 3>(0.0f, -1.0f, 0.0f), Vector<float, 2>(1.0f,  0.0f),
			Vector<float, 3>(posX + size, posY,		  posZ + size),  Vector<float, 3>(0.0f, -1.0f, 0.0f), Vector<float, 2>(1.0f,  1.0f),
			Vector<float, 3>(posX,		 posY,	      posZ + size),  Vector<float, 3>(0.0f, -1.0f, 0.0f), Vector<float, 2>(0.0f,  1.0f),

			Vector<float, 3>(posX,		 posY + size, posZ),		 Vector<float, 3>(0.0f, 1.0f, 0.0f)	, Vector<float, 2>(0.0f,  0.0f),
			Vector<float, 3>(posX + size, posY + size, posZ),		 Vector<float, 3>(0.0f, 1.0f, 0.0f)	, Vector<float, 2>(1.0f,  0.0f),
			Vector<float, 3>(posX + size, posY + size, posZ + size), Vector<float, 3>(0.0f, 1.0f, 0.0f)	, Vector<float, 2>(1.0f,  1.0f),
			Vector<float, 3>(posX,		 posY + size, posZ + size),  Vector<float, 3>(0.0f, 1.0f, 0.0f)	, Vector<float, 2>(0.0f,  1.0f)
		};
	}
	std::vector<unsigned int> CreateCube_Indices(void)
	{
		return {
			0,  1,  2,  2,  3,  0,
			4,  5,  6,  6,  7,  4,
			8,  9,  10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};
	}
}
