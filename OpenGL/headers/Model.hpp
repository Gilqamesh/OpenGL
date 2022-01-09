#pragma once
#ifndef MODEL_HPP
# define MODEL_HPP

# include <string>
# include "Shader.hpp"
# include "Mesh.hpp"
# include <vector>
# include <assimp/Importer.hpp>
# include <assimp/scene.h>
# include <assimp/postprocess.h>

class Model
{
public:
	Model();
	Model(const std::string& path);
	~Model();

	void draw(Shader& shader);
private:
	Model(const Model& m);
	Model& operator=(const Model& m);

	std::vector<Mesh>			meshes;
	std::string					directory;

	std::vector<Mesh::Vertex>			vertices;
	std::vector<unsigned int>			indices;

	std::vector<Texture>				ambientMaps;
	std::vector<Texture>				diffuseMaps;
	std::vector<Texture>				specularMaps;
	std::vector<Texture>				emissionMaps;
	Material							m_material;

	void					loadModel(const std::string& path);
	void					processNode(aiNode *node, const aiScene *scene);
	Mesh					processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture>	loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};

#endif
