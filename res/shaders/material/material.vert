#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normalVec;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

out vec3 fragmentPosition;
out vec3 v_normalVec;
out vec2 v_TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	v_TexCoord = a_TexCoord;
	fragmentPosition = vec3(model * vec4(position, 1.0f));
	v_normalVec = mat3(normalMatrix) * normalVec;
}
