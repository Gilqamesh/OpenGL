#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
}

#shader fragment
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
