#include "Model.hpp"
#include "Utils.hpp"

Model::Model()
{
}

Model::Model(const std::string& path)
{
	loadModel(path);
}

//Model::Model(const Model& m)
//{
//}
//
//Model& Model::operator=(const Model& m)
//{
//	// TODO: insert return statement here
//}

Model::~Model()
{
}

void Model::draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
		meshes[i].drawMesh(shader);
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer	import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Mesh::Vertex	vertex;
		// process vertex positions, normals and texture coordinates
		vertex.position[0] = mesh->mVertices[i].x;
		vertex.position[1] = mesh->mVertices[i].y;
		vertex.position[2] = mesh->mVertices[i].z;
		vertex.normal[0] = mesh->mNormals[i].x;
		vertex.normal[1] = mesh->mNormals[i].y;
		vertex.normal[2] = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertex.texCoord[0] = mesh->mTextureCoords[0][i].x;
			vertex.texCoord[1] = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.texCoord = Vector<float, 2>(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) // should be 3 if the aiProcess_Triangulate flag is enabled
			indices.push_back(face.mIndices[j]);
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
		diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
		specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
		emissionMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE);
		
		// Do diffuse map for now
		for (unsigned int i = 0; i < diffuseMaps.size(); ++i)
			m_material.addTexture(diffuseMaps[i]);
	}
	else
	{
		PRINT_HERE();
		std::cerr << "No index to texture map has been found for mesh, please handle this case!" << std::endl;
		exit(1);
	}

	return Mesh(vertices, indices, m_material);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture>	textures;

	std::cout << "loadMaterialTextures:\n";
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::cout << directory + "/" + std::string(str.C_Str()) << std::endl;
		Texture texture(directory + "/" + std::string(str.C_Str()));
		textures.push_back(texture);
	}
	std::cout << "end of loadMaterialTextures\n\n";

	return (textures);
}
