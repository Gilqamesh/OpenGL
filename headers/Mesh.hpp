#ifndef MESH_HPP
# define MESH_HPP

# include <vector>
# include "Material.hpp"
# include "VertexArray.hpp"
# include "VertexBuffer.hpp"
# include "VertexBufferLayout.hpp"
# include "IndexBuffer.hpp"
# include "Shader.hpp"
# include <array>

class Mesh
{
public:
	struct Vertex
	{
		Vertex() { }
		Vertex(const Vector<float, 3> &p, const Vector<float, 3> &n, const Vector<float, 2> &t)
			: position(p), normal(n), texCoord(t) { }
		Vector<float, 3>	position;
		Vector<float, 3>	normal;
		Vector<float, 2>	texCoord;
	};

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material& material);
	Mesh(const Mesh &m);
	~Mesh();

	void drawMesh(Shader&);
private:
	Mesh();

	const std::vector<Vertex>			&m_Vertices;
	const std::vector<unsigned int>		&m_Indices;
	Material							&m_Material;

	std::unique_ptr<VertexArray>		m_VAO;
	std::unique_ptr<VertexBuffer>		m_VBO;
	std::unique_ptr<IndexBuffer>		m_EBO;

	void configureMesh(void);
};

#endif
