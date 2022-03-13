#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material& material)
	: m_Vertices(vertices), m_Indices(indices), m_Material(material)
{
	configureMesh();
}

//const std::vector<Vertex>& m_Vertices;
//const std::vector<unsigned int>& m_Indices;
//Material& m_Material;
//
//std::unique_ptr<VertexArray>		m_VAO;
//std::unique_ptr<VertexBuffer>		m_VBO;
//std::unique_ptr<IndexBuffer>		m_EBO;

Mesh::Mesh(const Mesh& m)
	: m_Vertices(m.m_Vertices), m_Indices(m.m_Indices), m_Material(m.m_Material)
{
	configureMesh();
}

Mesh::~Mesh()
{
}

void Mesh::drawMesh(Shader& shader)
{
    TRACE();
	m_VAO->Bind();
	m_VBO->Bind();
	m_EBO->Bind();
	m_Material.bindTextures();
	m_Material.setTextureUniforms(shader);
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
	m_VAO->Unbind();
	m_VBO->Unbind();
	m_EBO->Unbind();
	m_Material.unbindTextures();
}

void Mesh::configureMesh(void)
{
    TRACE();
	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	// Either first or second param isn't right..
	m_VBO = std::make_unique<VertexBuffer>(m_Vertices.size() * sizeof(Vertex), m_Vertices.data());
	m_EBO = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size());
	
	// Depends on the Vertex structure
	VertexBufferLayout	vertexLayout;
	vertexLayout.Push<float>(3);
	vertexLayout.Push<float>(3);
	vertexLayout.Push<float>(2);
	m_VAO->AddBuffer(*m_VBO, vertexLayout);

	m_VAO->Unbind();
	m_VBO->Unbind();
	m_EBO->Unbind();
}
