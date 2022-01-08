#include "LightSource.hpp"

LightSource::LightSource(LightType type, const Vector<float, 3>& position, const Vector<float, 3>& color,
	float ambientFactor, float diffuseFactor, float specularFactor,
	float attenuationFactor_Constant, float attenuationFactor_Linear, float attenuationFactor_Quadratic)
	: type(type), position(position), color(color),
	ambientFactor(ambientFactor), diffuseFactor(diffuseFactor), specularFactor(specularFactor),
	attenuation(attenuationFactor_Constant, attenuationFactor_Linear, attenuationFactor_Quadratic)
{

}

LightSource::LightSource(LightType type, const Vector<float, 3>& position, const Vector<float, 3>& color, const Vector<float, 3>& direction,
	float innerCutOffAngle, float outerCutOffAngle,
	float ambientFactor, float diffuseFactor, float specularFactor,
	float attenuationFactor_Constant, float attenuationFactor_Linear, float attenuationFactor_Quadratic)
	: type(type), position(position), color(color), direction(direction),
	innerCutOffAngle(innerCutOffAngle), outerCutOffAngle(outerCutOffAngle),
	ambientFactor(ambientFactor), diffuseFactor(diffuseFactor), specularFactor(specularFactor),
	attenuation(attenuationFactor_Constant, attenuationFactor_Linear, attenuationFactor_Quadratic)
{
}

LightSource::LightSource(const LightSource& l)
	: type(l.type), position(l.position), color(l.color), direction(l.direction),
	innerCutOffAngle(l.innerCutOffAngle), outerCutOffAngle(l.outerCutOffAngle),
	ambientFactor(l.ambientFactor), diffuseFactor(l.diffuseFactor), specularFactor(l.specularFactor),
	attenuation(l.attenuation)
{

}

LightSource& LightSource::operator=(const LightSource& l)
{
	if (this != &l)
	{
		type = l.type;
		position = l.position;
		color = l.color;
		direction = l.direction;
		innerCutOffAngle = l.innerCutOffAngle;
		outerCutOffAngle = l.outerCutOffAngle;
		ambientFactor = l.ambientFactor;
		diffuseFactor = l.diffuseFactor;
		specularFactor = l.specularFactor;
		attenuation = l.attenuation;
	}
	return (*this);
}

LightSource::~LightSource()
{

}
