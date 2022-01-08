#version 330 core
#define LightType_Point			0
#define LightType_Directional	1
#define LightType_SpotLight		2

layout(location = 0) out vec4 color;

struct Light
{
	int type;
	vec3 position;
	vec3 color;
	vec3 direction;
	float innerCutOffAngle;
	float outerCutOffAngle;
	float ambientFactor;
	float diffuseFactor;
	float specularFactor;
	float attenuation_factor_constant;
	float attenuation_factor_linear;
	float attenuation_factor_quadratic;
};

uniform Light light;

void main()
{
	color = vec4(light.color, 1.0f);
}
