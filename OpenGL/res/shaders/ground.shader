#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normalVec;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 normalMatrix;

out vec4 v_color;
out vec3 v_normalVec;
out vec3 fragmentPosition;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	v_color = color;
	fragmentPosition = vec3(model * vec4(position, 1.0f));
	v_normalVec = mat3(normalMatrix) * normalVec;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec3 fragmentPosition;
in vec3 v_normalVec;

uniform vec4 u_LightColor;
uniform float ambientStrength;
uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform float specularStrength;
uniform float shininess;

void main()
{
	// Ambient
	vec4 ambient = vec4(vec3(u_LightColor) * ambientStrength, 1.0f);

	// Diffuse
	vec3 norm = normalize(v_normalVec);
	vec3 lightDir = normalize(lightPosition - fragmentPosition);
	vec4 diffuse = max(dot(norm, lightDir), 0.0f) * u_LightColor;

	// Specular
	vec3 viewDir = normalize(viewPos - fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec4 specular = specularStrength * spec * u_LightColor;

	color = (ambient + diffuse + specular) * v_color;
}
