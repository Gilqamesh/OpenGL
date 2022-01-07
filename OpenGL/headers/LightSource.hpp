#pragma once
#ifndef LIGHTSOURCE_HPP
# define LIGHTSOURCE_HPP

# include "math/Matrix.hpp"

class LightSource
{
public:
	enum class LightType {
		POINT,
		DIRECTIONAL
	};

	LightSource() { }
	LightSource(LightType type, const Vector<float, 3>& position, const Vector<float, 3>& ambientColor, const Vector<float, 3>& diffuseColor,
		const Vector<float, 3>& specularColor, float attenuationFactor_Constant = 1.0f, float attenuationFactor_Linear = 0.07f, float attenuationFactor_Quadratic = 0.017f);
	LightSource(const LightSource& l);
	LightSource& operator=(const LightSource &l);
	~LightSource();

	void			  setPosition(const Vector<float, 3>& v)	  { position = v;				     }
	Vector<float, 3>& setPosition()								  { return (position);			     }
	void			  setAmbientColor(const Vector<float, 3>& v)  { ambientColor = v;			     }
	Vector<float, 3>& setAmbientColor(void)						  { return (ambientColor);			 }
	void			  setDiffuseColor(const Vector<float, 3>& v)  { diffuseColor = v;			     }
	Vector<float, 3>& setDiffuseColor(void)						  { return (diffuseColor);			 }
	void			  setSpecularColor(const Vector<float, 3>& v) { specularColor = v;			     }
	Vector<float, 3>& setSpecularColor(void)					  { return (specularColor);			 }
	void			  setAttenuationFactors(float constant, float linear, float quadratic)
	{
		attenuation.constant = constant;
		attenuation.linear = linear;
		attenuation.quadratic = quadratic;
	}
	void			  setAttenuationFactor_Constant(float value)  { attenuation.constant = value;    }
	void			  setAttenuationFactor_Linear(float value)	  { attenuation.constant = value;    }
	void			  setAttenuationFactor_Quadratic(float value) { attenuation.constant = value;    }

	inline int						 getType(void)					const { return (static_cast<int>(type)); }
	inline const Vector<float, 3>&   getPosition(void)				const { return (position);			     }
	inline const Vector<float, 3>&   getAmbientColor(void)			const { return (ambientColor);		     }
	inline const Vector<float, 3>&   getDiffuseColor(void)			const { return (diffuseColor);		     }
	inline const Vector<float, 3>&   getSpecularColor(void)			const { return (specularColor);		     }
	inline float					 getAttenuation_Constant(void)	const { return (attenuation.constant);   }
	inline float					 getAttenuation_Linear(void)	const { return (attenuation.constant);   }
	inline float					 getAttenuation_Quadratic(void)	const { return (attenuation.constant);   }
private:
	class attenuationFactor
	{
	public:
		attenuationFactor() { }
		attenuationFactor(float c, float l, float q) : constant(c), linear(l), quadratic(q) { }
		attenuationFactor(const attenuationFactor &a) : constant(a.constant), linear(a.linear), quadratic(a.quadratic) { }
		attenuationFactor& operator=(const attenuationFactor& a)
		{
			if (this != &a)
			{
				constant = a.constant;
				linear = a.linear;
				quadratic = a.quadratic;
			}
			return (*this);
		}
		~attenuationFactor() { }

		float constant;
		float linear;
		float quadratic;
	};
	
	LightType			type;
	Vector<float, 3>	position;
	Vector<float, 3>	ambientColor;
	Vector<float, 3>	diffuseColor;
	Vector<float, 3>	specularColor;
	attenuationFactor	attenuation;
};

#endif
