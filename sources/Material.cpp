#include "Material.hpp"
#include "Shader.hpp"

Material::Material()
	: shininess(0.0f)
{

}

Material::~Material()
{
	textures.clear();
}

Material::Material(float shininessFactor, const Texture& ambient, const Texture& diffuse, const Texture& specular, const Texture& emission)
	: shininess(shininessFactor)
{
	textures.push_back({
		&ambient,
		&diffuse,
		&specular,
		&emission
	});
}

Material::Material(float shininessFactor, const Texture& ambient)
	: Material(shininessFactor, ambient, ambient)
{

}

Material::Material(float shininessFactor)
	: shininess(shininessFactor)
{

}

Material::Material(float shininessFactor, const std::vector<colorMaps>& textureMaps)
	: shininess(shininessFactor), textures(textureMaps)
{
}

Material::Material(const Material& m)
	: shininess(m.shininess), textures(m.textures)
{

}

Material& Material::operator=(const Material& m)
{
	if (this != &m)
	{
		shininess	= m.shininess;
		textures	= m.textures;
	}
	return (*this);
}

void Material::clearTextures(void)
{
    TRACE();
	textures.clear();
}

void Material::setTextureUniforms(Shader& shader) const
{
    TRACE();
	shader.SetUniform1i("material.mapsSize", textures.size());
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		if (textures[i].ambient->isLoadedFromFile() == true)
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].ambient.type", 1);
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].ambient.tex", textures[i].ambientTextureSlot);
		}
		else
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].ambient.type", 0);
			shader.SetUniform4f("material.textures[" + std::to_string(i) + "].ambient.color", textures[i].ambient->getColor());
		}

		if (textures[i].diffuse->isLoadedFromFile() == true)
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].diffuse.type", 1);
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].diffuse.tex", textures[i].diffuseTextureSlot);
		}
		else
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].diffuse.type", 0);
			shader.SetUniform4f("material.textures[" + std::to_string(i) + "].diffuse.color", textures[i].diffuse->getColor());
		}

		if (textures[i].specular->isLoadedFromFile() == true)
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].specular.type", 1);
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].specular.tex", textures[i].specularTextureSlot);
		}
		else
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].specular.type", 0);
			shader.SetUniform4f("material.textures[" + std::to_string(i) + "].specular.color", textures[i].specular->getColor());
		}

		if (textures[i].emission->isLoadedFromFile() == true)
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].emission.type", 1);
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].emission.tex", textures[i].emissionTextureSlot);
		}
		else
		{
			shader.SetUniform1i("material.textures[" + std::to_string(i) + "].emission.type", 0);
			shader.SetUniform4f("material.textures[" + std::to_string(i) + "].emission.color", textures[i].emission->getColor());
		}
	}
	shader.SetUniform1f("material.shininess", shininess);
}

void Material::addTexture(const Texture& ambient, const Texture& diffuse, const Texture& specular, const Texture& emission)
{
    TRACE();
	textures.push_back({ &ambient, &diffuse, &specular, &emission });
}

void Material::addTexture(const Texture& ambient)
{
    TRACE();
	addTexture(ambient, ambient);
}

void Material::bindTextures(void)
{
    TRACE();
	unsigned int	currentSlot = 0;

	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		if (textures[i].ambient->isLoadedFromFile())
		{
			textures[i].ambient->Bind(currentSlot);
			textures[i].ambientTextureSlot = currentSlot++;
		}
		if (textures[i].diffuse->isLoadedFromFile())
		{
			textures[i].diffuse->Bind(currentSlot);
			textures[i].diffuseTextureSlot = currentSlot++;
		}
		if (textures[i].specular->isLoadedFromFile())
		{
			textures[i].specular->Bind(currentSlot);
			textures[i].specularTextureSlot = currentSlot++;
		}
		if (textures[i].emission->isLoadedFromFile())
		{
			textures[i].emission->Bind(currentSlot);
			textures[i].emissionTextureSlot = currentSlot++;
		}
	}
}

void Material::unbindTextures(void) const
{
    TRACE();
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
