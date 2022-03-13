#ifndef LIGHTSOURCE_HPP
# define LIGHTSOURCE_HPP

# include "math/Matrix.hpp"

class LightSource
{
public:
	enum class LightType {
		POINT,
		DIRECTIONAL,
		SPOTLIGHT
	};

	LightSource() { }
	LightSource(LightType type, const Vector<float, 3>& position, const Vector<float, 4>& color,
		float ambientFactor, float diffuseFactor, float specularFactor,
		float attenuationFactor_Constant = 1.0f, float attenuationFactor_Linear = 0.07f, float attenuationFactor_Quadratic = 0.017f);
	LightSource(LightType type, const Vector<float, 3>& position, const Vector<float, 4>& color, const Vector<float, 3>& direction,
		float innerCutOffAngle, float outerCutOffAngle, float ambientFactor, float diffuseFactor, float specularFactor,
		float attenuationFactor_Constant = 1.0f, float attenuationFactor_Linear = 0.07f, float attenuationFactor_Quadratic = 0.017f);
	LightSource(const LightSource& l);
	LightSource& operator=(const LightSource &l);
	~LightSource();

	void			  setPosition(const Vector<float, 3>& value)	  { position = value;				     }
	void			  setColor(const Vector<float, 4>& value) { color = value; }
	void			  setDirection(const Vector<float, 3>& value) { direction = value; }
	void			  setInnerCutOffAngle(float value) { innerCutOffAngle = value; }
	void			  setOuterCutOffAngle(float value) { outerCutOffAngle = value; }
	void			  setAmbientFactor(float value)  { ambientFactor = value; }
	void			  setDiffuseFactor(float value)  { diffuseFactor = value; }
	void			  setSpecularFactor(float value) { specularFactor = value; }
	void			  setAttenuationFactors(float constant, float linear, float quadratic)
	{
		attenuation.constant = constant;
		attenuation.linear = linear;
		attenuation.quadratic = quadratic;
	}
	void			  setAttenuationFactor_Constant(float value)  { attenuation.constant = value;    }
	void			  setAttenuationFactor_Linear(float value)	  { attenuation.linear = value;      }
	void			  setAttenuationFactor_Quadratic(float value) { attenuation.quadratic = value;   }

	Vector<float, 3>& setPosition(void)								  { return (position);				 }
	Vector<float, 4>& setColor(void) { return (color); }
	Vector<float, 3>& setDirection(void) { return (direction); }
	float&			  setInnerCutOffAngle(void) { return (innerCutOffAngle); }
	float&			  setOuterCutOffAngle(void) { return (outerCutOffAngle); }
	float&			  setAmbientFactor(void)					  { return (ambientFactor);			 }
	float&			  setDiffuseFactor(void)						  { return (diffuseFactor);			 }
	float&			  setSpecularFactor(void)					  { return (specularFactor);		 }
	float*			  setAttenuationFactors(void)				  { return (&attenuation.constant);  } // need test
	float&			  setAttenuationFactor_Constant(void)		  { return (attenuation.constant);   }
	float&			  setAttenuationFactor_Linear(void)			  { return (attenuation.linear);     }
	float&			  setAttenuationFactor_Quadratic(void)		  { return (attenuation.quadratic);  }

	inline int						 getType(void)					const { return (static_cast<int>(type)); }
	inline const Vector<float, 4>& getColor(void) const { return (color); }
	inline const Vector<float, 3>& getDirection(void) const { return (direction); }
	inline float getInnerCutOffAngle(void) const { return (innerCutOffAngle); }
	inline float getOuterCutOffAngle(void) const { return (outerCutOffAngle); }
	inline const Vector<float, 3>&   getPosition(void)				const { return (position);			     }
	inline float   getAmbientFactor(void)			const { return (ambientFactor);		     }
	inline float   getDiffuseFactor(void)			const { return (diffuseFactor);		     }
	inline float   getSpecularFactor(void)			const { return (specularFactor);		     }
	inline float					 getAttenuation_Constant(void)	const { return (attenuation.constant);   }
	inline float					 getAttenuation_Linear(void)	const { return (attenuation.linear);     }
	inline float					 getAttenuation_Quadratic(void)	const { return (attenuation.quadratic);  }
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
	Vector<float, 4>	color;
	Vector<float, 3>	direction;
	float				innerCutOffAngle;
	float				outerCutOffAngle;
	float				ambientFactor;
	float				diffuseFactor;
	float				specularFactor;
	attenuationFactor	attenuation;
};

#endif
