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
#define MaterialColor_Tex 0
#define MaterialColor_Normal 1
#define LightType_Point 0
#define LightType_Directional 1
#define LightType_SpotLight 2

struct Material
{
	int colorType;
	sampler2D diffuseMap;
	sampler2D specularMap;
	int hasSpecularMap;
	sampler2D emissionMap;
	int hasEmissionMap;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininessFactor;
};

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

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 fragmentPosition;
in vec3 v_normalVec;

uniform Light		light;
uniform Material	material;
uniform vec3		viewPos;

void main()
{
	// New code
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	if (material.colorType == MaterialColor_Tex)
	{
		// Ambient
		ambient = light.ambientFactor * light.color * texture(material.diffuseMap, v_TexCoord).rgb;

		// Diffuse
		vec3	norm		  = normalize(v_normalVec);
		vec3	lightDir	  = normalize(light.position - fragmentPosition);
		float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
				diffuse		  = light.diffuseFactor * light.color * diffuseFactor * texture(material.diffuseMap, v_TexCoord).rgb;

		// Specular
		vec3	viewDir			 = normalize(viewPos - fragmentPosition);
		vec3	reflectDir		 = reflect(-lightDir, norm);
		float	specularFactor	 = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
		vec3 specular			 = light.specularFactor * light.color * specularFactor * texture(
			(material.specularMap != -1 ? material.specularMap : material.diffuseMap), v_TexCoord).rgb;

		// Emission
		vec3 emission = (material.emissionMap != -1 ? texture(material.emissionMap, v_TexCoord).rgb : vec3(0.0f, 0.0f, 0.0f));

		float intensity = 1.0f;
		// Spotlight
		if (light.type == LightType_SpotLight)
		{
			float theta = dot(lightDir, normalize(-light.direction));
			float epsilon = light.innerCutOffAngle - light.outerCutOffAngle;
			intensity = clamp((theta - light.outerCutOffAngle) / epsilon, 0.0f, 1.0f);
			if (theta < light.outerCutOffAngle)
			{
				diffuse = vec3(0.0f, 0.0f, 0.0f);
				specular = vec3(0.0f, 0.0f, 0.0f);
			}
		}

		// Attenuation factor
		float	distance = length(light.position - fragmentPosition);
		float	attenuation_factor = 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
							  * light.attenuation_factor_quadratic * distance * distance);
		ambient  *= attenuation_factor;
		diffuse  *= attenuation_factor * intensity;
		specular *= attenuation_factor * intensity;

		color = vec4(ambient + diffuse + specular + emission, 1.0f);
	}
	else if (material.colorType == MaterialColor_Normal)
	{
		// Ambient
		ambient = light.ambientFactor * light.color * material.ambientColor;

		// Diffuse
		vec3	norm		  = normalize(v_normalVec);
		vec3	lightDir	  = normalize(light.position - fragmentPosition);
		float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
				diffuse		  = light.diffuseFactor * light.color * diffuseFactor * vec3(material.diffuseColor);

		// Specular
		vec3	viewDir		   = normalize(viewPos - fragmentPosition);
		vec3	reflectDir	   = reflect(-lightDir, norm);
		float	specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
		vec3	specular	   = light.specularFactor * light.color * specularFactor * material.specularColor;

		float intensity = 1.0f;
		// Spotlight
		if (light.type == LightType_SpotLight)
		{
			float theta = dot(lightDir, normalize(-light.direction));
			float epsilon = light.innerCutOffAngle - light.outerCutOffAngle;
			intensity = clamp((theta - light.outerCutOffAngle) / epsilon, 0.0f, 1.0f);
			if (theta < light.outerCutOffAngle)
			{
				diffuse = vec3(0.0f, 0.0f, 0.0f);
				specular = vec3(0.0f, 0.0f, 0.0f);
			}
		}

		// Attenuation factor
		float	distance = length(light.position - fragmentPosition);
		float	attenuation_factor = 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
							  * light.attenuation_factor_quadratic * distance * distance);
		ambient  *= attenuation_factor;
		diffuse  *= attenuation_factor * intensity;
		specular *= attenuation_factor * intensity;

		color = vec4(ambient + diffuse + specular, 1.0f);
	}
}
