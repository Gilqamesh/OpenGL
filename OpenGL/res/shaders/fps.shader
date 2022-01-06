#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float texSlot;
layout(location = 3) in vec3 normalVec;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

out vec2 v_TexCoord;
out vec3 v_normalVec;
out vec3 fragmentPosition;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	v_TexCoord = a_TexCoord;
	fragmentPosition = vec3(model * vec4(position, 1.0f));
	v_normalVec = mat3(normalMatrix) * normalVec;
}

#shader fragment
#version 330 core

struct Material
{
	enum colorType
	{
		TEX,
		COLOR
	};
	union
	{
		struct
		{
			sampler2D tex;
			sampler2D texSpecular;
			sampler2D texEmission;
		} t;
		vec4 color;
	} color;

	vec3	  specularColor;
	float	  specularStrength;

	float	  shininessFactor;
};

struct Light
{
	enum type {
		POINT,
		DIRECTIONAL
	};

	vec3 position;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	struct
	{
		float constant;
		float linear;
		float quadratic;
	} attenuation_factor;
};

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 fragmentPosition;
in vec3 v_normalVec;

uniform Light		light;
uniform Material	material;
uniform vec3		viewPos;
uniform vec3		lightPosition;

// Old Code
//uniform sampler2D u_Texture;
//uniform sampler2D u_TextureSpecular;
//uniform sampler2D u_TextureEmission;
//uniform vec4 u_LightColor;
//uniform float ambientStrength;
//uniform vec3 lightPosition;
//uniform vec3 viewPos;
//uniform float specularStrength;
//uniform float shininess;

void main()
{
	// Old code
	// Ambient
	// vec3 ambient = vec3(u_LightColor) * ambientStrength * texture(u_Texture, v_TexCoord).rgb;
	// Diffuse
	// vec3 norm = normalize(v_normalVec);
	// vec3 lightDir = normalize(lightPosition - fragmentPosition);
	// float diffStrength = max(dot(norm, lightDir), 0.0f);
	// vec3 diffuse = vec3(u_LightColor) * diffStrength * texture(u_Texture, v_TexCoord).rgb;
	// Specular
	// vec3 viewDir = normalize(viewPos - fragmentPosition);
	// vec3 reflectDir = reflect(-lightDir, norm);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	//vec3 specular = specularStrength * spec * vec3(u_LightColor) * texture(u_TextureSpecular, v_TexCoord).rgb;

	// New code
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	if (material.colorType == TEX)
	{
		// Ambient
		ambient = light.ambientColor * texture(material.color.t.tex, v_TexCoord).rgb;

		// Diffuse
		vec3	norm = normalize(v_normalVec);
		vec3	lightDir = normalize(light.position - fragmentPosition);
		float	diffuseStrength = max(dot(norm, lightDir), 0.0f);
				diffuse = light.diffuseColor * diffuseStrength * texture(material.color.t.tex, v_TexCoord).rgb;

		// Specular
		vec3	viewDir = normalize(viewPos - fragmentPosition);
		vec3	reflectDir = reflect(-lightDir, norm);
		float	specularStrength = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
		vec3	specular = light.specularColor * specularStrength * material.specularColor;

		// Emission
		vec3	emission = texture(material.color.t.texEmission, v_TexCoord).rgb;

		color = vec4(ambient + diffuse + specular + emission, 1.0f);
	}
	else if (material.colorType == COLOR)
	{
		// Ambient
		ambient = light.ambientColor * vec3(material.color.color);

		// Diffuse
		vec3	norm = normalize(v_normalVec);
		vec3	lightDir = normalize(light.position - fragmentPosition);
		float	diffuseStrength = max(dot(norm, lightDir), 0.0f);
				diffuse = light.diffuseColor * diffuseStrength * vec3(material.color.color);

		// Specular
		vec3	viewDir = normalize(viewPos - fragmentPosition);
		vec3	reflectDir = reflect(-lightDir, norm);
		float	specularStrength = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
		vec3	specular = light.specularColor * specularStrength * material.specularColor;

		color = vec4(ambient + diffuse + specular, 1.0f);
	}
}
