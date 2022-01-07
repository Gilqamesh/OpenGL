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

# define MaterialColor_Tex		0
# define MaterialColor_Normal	1

struct Material
{
	int colorType;
	union
	{
		struct
		{
			sampler2D diffuseMap;
			sampler2D specularMap;
			sampler2D emissionMap;
		} lightingMaps;
		struct
		{
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 specularColor;
		} color;
	} color;

	float	  specularStrength;
	float	  shininessFactor;
};

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

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 fragmentPosition;
in vec3 v_normalVec;

uniform Light		light;
uniform Material	material;
uniform vec3		viewPos;

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

	if (material.colorType == MaterialColor_Tex)
	{
		// Ambient
		ambient = light.ambientColor * texture(material.color.lightingMaps.diffuseMap, v_TexCoord).rgb;

		// Diffuse
		vec3	norm		  = normalize(v_normalVec);
		vec3	lightDir	  = normalize(light.position - fragmentPosition);
		float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
				diffuse		  = light.diffuseColor * diffuseFactor * texture(material.color.lightingMaps.diffuseMap, v_TexCoord).rgb;

		// Specular
		vec3	viewDir			 = normalize(viewPos - fragmentPosition);
		vec3	reflectDir		 = reflect(-lightDir, norm);
		float	specularFactor	 = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
		vec3	specular		 = light.specularColor * specularFactor * material.color.lightingMaps.specularMap;

		// Emission
		vec3	emission = texture(material.color.lightingMaps.emissionMap, v_TexCoord).rgb;

		color = vec4(ambient + diffuse + specular + emission, 1.0f);
	}
	else if (material.colorType == MaterialColor_Normal)
	{
		// Ambient
		ambient = light.ambientColor * material.color.color.ambientColor;

		// Diffuse
		vec3	norm		  = normalize(v_normalVec);
		vec3	lightDir	  = normalize(light.position - fragmentPosition);
		float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
				diffuse		  = light.diffuseColor * diffuseFactor * vec3(material.color.color.diffuseColor);

		// Specular
		vec3	viewDir		   = normalize(viewPos - fragmentPosition);
		vec3	reflectDir	   = reflect(-lightDir, norm);
		float	specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
		vec3	specular	   = light.specularColor * specularFactor * material.color.color.specularColor;

		color = vec4(ambient + diffuse + specular, 1.0f);
	}
}
