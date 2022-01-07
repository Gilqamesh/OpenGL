#include "LightSource.hpp"

LightSource::LightSource(LightType type, const Vector<float, 3>& position, const Vector<float, 3>& ambientColor, const Vector<float, 3>& diffuseColor, const Vector<float, 3>& specularColor, float attenuationFactor_Constant, float attenuationFactor_Linear, float attenuationFactor_Quadratic)
	: type(type), position(position), ambientColor(ambientColor), diffuseColor(diffuseColor), specularColor(specularColor), attenuation(attenuationFactor_Constant, attenuationFactor_Linear, attenuationFactor_Quadratic)
{

}

LightSource::LightSource(const LightSource& l)
	: type(l.type), position(l.position), ambientColor(l.ambientColor), diffuseColor(l.diffuseColor), specularColor(l.specularColor), attenuation(l.attenuation)
{

}

LightSource& LightSource::operator=(const LightSource& l)
{
	if (this != &l)
	{
		type = l.type;
		position = l.position;
		ambientColor = l.ambientColor;
		diffuseColor = l.diffuseColor;
		specularColor = l.specularColor;
		attenuation = l.attenuation;
	}
	return (*this);
}

LightSource::~LightSource()
{

}
