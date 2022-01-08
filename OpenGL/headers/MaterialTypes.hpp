#pragma once
#ifndef MATERIALTYPES_HPP
# define MATERIALTYPES_HPP

# include "math/Matrix.hpp"
# include <cstring>

class Material
{
public:
	enum class colorType
	{
		TEX,
		COLOR
	};

	Material() { }
	Material(colorType type, float shininessFactor, const Vector<float, 3>& ambient, const Vector<float, 3>& diffuse, const Vector<float, 3>& specular, const Vector<float, 3>& emission)
		: m_colorType(type), unionColor(ambient, diffuse, specular, emission), shininessFactor(shininessFactor) { }
	Material(colorType type, float shininessFactor, const Vector<float, 3>& ambient, const Vector<float, 3>& diffuse, const Vector<float, 3> &specular)
		: m_colorType(type), unionColor(ambient, diffuse, specular), shininessFactor(shininessFactor) { }
	Material(colorType type, float shininessFactor, const Vector<float, 3>& ambient, const Vector<float, 3>& diffuse)
		: m_colorType(type), unionColor(ambient, diffuse, Vector<float, 3>(0.5f, 0.5f, 0.5f)), shininessFactor(shininessFactor) { }
	Material(colorType type, float shininessFactor, const Vector<float, 3>& ambient)
		: m_colorType(type), unionColor(ambient, ambient, Vector<float, 3>(0.5f, 0.5f, 0.5f)), shininessFactor(shininessFactor) { }

	Material(colorType type, float shininessFactor, int diffuse = -1, int specular = -1, int emission = -1);
	Material(const Material& m);
	Material& operator=(const Material& m);

	void setColors(const Vector<float, 3>& ambient, const Vector<float, 3>& diffuse, const Vector<float, 3>& specular)
	{
		unionColor.color.ambientColor = ambient;
		unionColor.color.diffuseColor = diffuse;
		unionColor.color.specularColor = specular;
	}
	void setColors(const Vector<float, 3>& colors)
	{
		unionColor.color.ambientColor = colors;
		unionColor.color.diffuseColor = colors;
		unionColor.color.specularColor = colors;
	}
	void setColors(int diffuse, int specular, int emission)
	{
		unionColor.maps.diffuseMap = diffuse;
		unionColor.maps.specularMap = specular;
		unionColor.maps.emissionMap = emission;
	}
	void setAmbientColor(const Vector<float, 3>& ambient)   { unionColor.color.ambientColor = ambient;   }
	void setDiffuseColor(const Vector<float, 3>& diffuse)   { unionColor.color.diffuseColor = diffuse;   }
	void setDiffuseMap(int diffuse)						    { unionColor.maps.diffuseMap = diffuse;      }
	void setSpecularColor(const Vector<float, 3>& specular) { unionColor.color.specularColor = specular; }
	void setSpecularMap(int specular)						{ unionColor.maps.specularMap = specular;	 }
	void setEmissionMap(int emission)						{ unionColor.maps.emissionMap = emission;	 }
	void setShininessFactor(float factor)				    { shininessFactor = factor;					 }

	Vector<float, 3>&	setAmbientColor(void)		{ return (unionColor.color.ambientColor);	}
	Vector<float, 3>&	setDiffuseColor(void)		{ return (unionColor.color.diffuseColor);	}
	int&				setDiffuseMap(void)			{ return (unionColor.maps.diffuseMap);		}
	Vector<float, 3>&	setSpecularColor(void)		{ return (unionColor.color.specularColor);	}
	int&				setSpecularMap(void)		{ return (unionColor.maps.specularMap);		}
	int&				setEmissionMap(void)		{ return (unionColor.maps.emissionMap);		}
	float&				setShininessFactor(void)	{ return (shininessFactor);					}

	inline int						getColorType(void)	      const { return (static_cast<int>(m_colorType));  }
	inline const Vector<float, 3>&  getAmbientColor(void)     const { return (unionColor.color.ambientColor);  }
	inline const Vector<float, 3>&  getDiffuseColor(void)     const { return (unionColor.color.diffuseColor);  }
	inline int						getDiffuseMap(void)		  const { return (unionColor.maps.diffuseMap);	   }
	inline const Vector<float, 3>&  getSpecularColor(void)    const { return (unionColor.color.specularColor); }
	inline int						getSpecularMap(void)	  const { return (unionColor.maps.specularMap);	   }
	inline int						getEmissionMap(void)	  const { return (unionColor.maps.emissionMap);	   }
	inline float					getShininessFactor(void)  const { return (shininessFactor);				   }
private:
	class lightingMaps
	{
	public:
		lightingMaps() { }
		lightingMaps(int diffuse, int specular, int emission)
			: diffuseMap(diffuse), specularMap(specular), emissionMap(emission) { }
		lightingMaps(const lightingMaps &l): diffuseMap(l.diffuseMap), specularMap(l.specularMap), emissionMap(l.emissionMap) { }
		lightingMaps &operator=(const lightingMaps &l)
		{
			if (this != &l)
			{
				diffuseMap = l.diffuseMap;
				specularMap = l.specularMap;
				emissionMap = l.emissionMap;
			}
		}

		int		diffuseMap;
		int		specularMap;
		int		emissionMap;
	};
	class materialColor
	{
	public:
		materialColor() { }
		materialColor(const Vector<float, 3>& ambientC, const Vector<float, 3>& diffuseC, const Vector<float, 3>& specularC, const Vector<float, 3>& emissionC = Vector<float, 3>(0.0f, 0.0f, 0.0f))
			: ambientColor(ambientC), diffuseColor(diffuseC), specularColor(specularC), emissionColor(emissionC) { }
		materialColor(const materialColor &m)
			: ambientColor(m.ambientColor), diffuseColor(m.diffuseColor), specularColor(m.specularColor), emissionColor(m.emissionColor) { }
		materialColor& operator=(const materialColor& m)
		{
			if (this != &m)
			{
				ambientColor = m.ambientColor;
				diffuseColor = m.diffuseColor;
				specularColor = m.specularColor;
				emissionColor = m.emissionColor;
			}
		}

		Vector<float, 3> ambientColor;
		Vector<float, 3> diffuseColor;
		Vector<float, 3> specularColor;
		Vector<float, 3> emissionColor;
	};
	union uColor
	{
		uColor() { }
		uColor(const Vector<float, 3>& ambient, const Vector<float, 3>& diffuse, const Vector<float, 3>& specular)
			: color(ambient, diffuse, specular) { }
		uColor(const Vector<float, 3>& ambient, const Vector<float, 3>& diffuse, const Vector<float, 3>& specular, const Vector<float, 3>& emission)
			: color(ambient, diffuse, specular, emission) { }
		uColor(int diffuse, int specular, int emission) : maps(diffuse, specular, emission) { }
		uColor(const uColor& u) { std::memcpy(this, &u, sizeof(uColor)); }
		uColor& operator=(const uColor& u) { if (this != &u) std::memcpy(this, &u, sizeof(uColor)); return (*this); }
		~uColor() { }

		lightingMaps	 maps;
		materialColor	 color;
	};

	colorType			 m_colorType;
	uColor				 unionColor;
	float				 shininessFactor;
};

// Old Material structure, need to rewrite these consts in cpp based on Material's constructor (diffuseColor and ambientColor is the same in
// Material as the AmbientColor is implemented from the LightSource perspective)
/*
** std::array<float, 3> ambientColor;
** std::array<float, 3> diffuseColor;
** std::array<float, 3> specularColor;
** float				shininess;
*/
struct MaterialTypes
{
	static const Material emerald;
	static const Material jade;
	static const Material obsidian;
	static const Material pearl;
	static const Material ruby;
	static const Material turquoise;
	static const Material brass;
	static const Material bronze;
	static const Material chrome;
	static const Material copper;
	static const Material gold;
	static const Material silver;
	static const Material black_plastic;
	static const Material cyan_plastic;
	static const Material green_plastic;
	static const Material red_plastic;
	static const Material white_plastic;
	static const Material yellow_plastic;
	static const Material black_rubber;
	static const Material cyan_rubber;
	static const Material green_rubber;
	static const Material red_rubber;
	static const Material white_rubber;
	static const Material yellow_rubber;
};

#endif
