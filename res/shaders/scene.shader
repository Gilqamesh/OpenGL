#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0f);
	v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
}
