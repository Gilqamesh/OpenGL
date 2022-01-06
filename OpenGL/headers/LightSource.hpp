#pragma once
#ifndef LIGHTSOURCE_HPP
# define LIGHTSOURCE_HPP

# include "math/Vector.hpp"

struct LightSource
{
	enum type {
		POINT,
		DIRECTIONAL
	};

	Vector<float, 3> position;

	Vector<float, 3> ambientColor;
	Vector<float, 3> diffuseColor;
	Vector<float, 3> specularColor;

	struct
	{
		float constant;
		float linear;
		float quadratic;
	} attenuation_factor;
};

#endif
