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

layout(location = 0) out vec4 color;

# define LightType_Point		0
# define LightType_Directional	1

struct Light
{
	int		type;
	vec3	position;

	vec3	ambientColor;
	vec3	diffuseColor;
	vec3	specularColor;

	struct
	{
		float constant;
		float linear;
		float quadratic;

	} attenuation_factor;
};

uniform Light light;

void main()
{
	color = light.specularColor;
}
