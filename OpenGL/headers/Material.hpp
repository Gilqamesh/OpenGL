#pragma once
#ifndef MATERIAL_HPP
# define MATERIAL_HPP

# include "Texture.hpp"
# include <functional>

class Shader;

class Material
{
public:
	struct colorMaps
	{
		const Texture* ambient;
		const Texture* diffuse;
		const Texture* specular;
		const Texture* emission;
		unsigned int	ambientTextureSlot;
		unsigned int	diffuseTextureSlot;
		unsigned int	specularTextureSlot;
		unsigned int	emissionTextureSlot;
	};

	Material();
	~Material();
	Material(float shininessFactor, const Texture& ambient, const Texture& diffuse, const Texture& specular = Vector<float, 4>(0.5f, 0.5f, 0.5f, 1.0f), const Texture& emission = Vector<float, 4>(0.0f, 0.0f, 0.0f, 0.0f));
	Material(float shininessFactor, const Texture& ambient);
	Material(float shininessFactor);

	Material(float shininessFactor, const std::vector<colorMaps>& textureMaps);

	Material(const Material& m);
	Material& operator=(const Material& m);

	void addTexture(const Texture& ambient, const Texture& diffuse, const Texture& specular = Vector<float, 4>(0.5f, 0.5f, 0.5f, 1.0f), const Texture& emission = Vector<float, 4>(0.0f, 0.0f, 0.0f, 1.0f));
	void addTexture(const Texture& ambient);
	void bindTextures(void);
	void unbindTextures(void) const;
	void clearTextures(void);

	void setTextureUniforms(Shader &shader) const;

	inline float&	setShininessFactor(void)			{ return (shininess); }
	inline void		setShininessFactor(float factor)	{ shininess = factor; }

	inline float	getShininessFactor(void) const { return (shininess); }
private:
	float					shininess;
	std::vector<colorMaps>	textures;
};

#endif
