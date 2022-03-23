#version 330 core
#define N_OF_LIGHTS 2
#define LightType_Point 0
#define LightType_Directional 1
#define LightType_Spot 2

#define MAX_MAPS 4
#define MaterialColor_Normal 0
#define MaterialColor_Tex 1

layout(location = 0) out vec4 color;

in vec3 fragmentPosition;
in vec3 v_normalVec;
in vec2 v_TexCoord;

struct mapType
{
	vec4		color;
	sampler2D	tex;
	int			type;
};

struct maps
{
	mapType ambient;
	mapType diffuse;
	mapType specular;
	mapType emission;
};

struct Material
{
	int			mapsSize;
	maps		textures[MAX_MAPS];
	float		shininess;
};

struct Light
{
	int type;
	vec3 position;
	vec4 color;
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

uniform vec3 viewPos;
uniform Light light[N_OF_LIGHTS];
uniform Material material;

vec4 calculatePointLightColor(Light light, int textureIndex);
vec4 calculateDirectionalLightColor(Light light, int textureIndex);
vec4 calculateSpotLightColor(Light light, int textureIndex);

void main()
{
	vec4 sumColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < N_OF_LIGHTS; ++i)
	{
		for (int j = 0; j < material.mapsSize; ++j)
		{
			if (light[i].type == LightType_Point)
				sumColor += calculatePointLightColor(light[i], j);
			else if (light[i].type == LightType_Directional)
				sumColor += calculateDirectionalLightColor(light[i], j);
			else if (light[i].type == LightType_Spot)
				sumColor += calculateSpotLightColor(light[i], j);
		}
	}
	color = vec4(sumColor.rgb, 1.0f);
}

vec4 calculatePointLightColor(Light light, int textureIndex)
{
	// Ambient
	vec4	ambient;
	if (material.textures[textureIndex].ambient.type == MaterialColor_Normal)
	{
		ambient = light.ambientFactor * light.color * material.textures[textureIndex].ambient.color;
	}
	else
	{
		ambient = light.ambientFactor * light.color * texture(material.textures[textureIndex].ambient.tex, v_TexCoord);
	}

	// Diffuse
	vec3	norm				= normalize(v_normalVec);
	vec3	lightDir			= normalize(light.position - fragmentPosition);
	float	diffuseFactor		= max(dot(norm, lightDir), 0.0f);
	vec4	diffuse;
	if (material.textures[textureIndex].diffuse.type == MaterialColor_Normal)
	{
		diffuse = light.diffuseFactor * light.color * diffuseFactor * material.textures[textureIndex].diffuse.color;
	}
	else
	{
		diffuse = light.diffuseFactor * light.color * diffuseFactor * texture(material.textures[textureIndex].diffuse.tex, v_TexCoord);
	}

	// Specular
	vec3	viewDir				= normalize(viewPos - fragmentPosition);
	vec3	reflectDir			= reflect(-lightDir, norm);
	float	shininessFactor		= pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec4	specular;
	if (material.textures[textureIndex].specular.type == MaterialColor_Normal)
	{
		specular = light.specularFactor * light.color * shininessFactor * material.textures[textureIndex].specular.color;
	}
	else
	{
		specular = light.specularFactor * light.color * shininessFactor * texture(material.textures[textureIndex].specular.tex, v_TexCoord);
	}

	// Emission
	vec4	emission;
	if (material.textures[textureIndex].emission.type == MaterialColor_Normal)
	{
		emission = material.textures[textureIndex].emission.color;
	}
	else
	{
		emission = texture(material.textures[textureIndex].emission.tex, v_TexCoord);
	}

	// Attenuation factor
	float	distance			= length(light.position - fragmentPosition);
	float	attenuation_factor	= 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
									* light.attenuation_factor_quadratic * distance * distance);

	return ((ambient + diffuse + specular + emission) * attenuation_factor);
}

vec4 calculateDirectionalLightColor(Light light, int textureIndex)
{
	// Ambient
	vec4	ambient;
	if (material.textures[textureIndex].ambient.type == MaterialColor_Normal)
	{
		ambient = light.ambientFactor * light.color * material.textures[textureIndex].ambient.color;
	}
	else
	{
		ambient = light.ambientFactor * light.color * texture(material.textures[textureIndex].ambient.tex, v_TexCoord);
	}

	// Diffuse
	vec3	norm				= normalize(v_normalVec);
	vec3	lightDir			= normalize(-light.direction);
	float	diffuseFactor		= max(dot(norm, lightDir), 0.0f);
	vec4	diffuse;
	if (material.textures[textureIndex].diffuse.type == MaterialColor_Normal)
	{
		diffuse = light.diffuseFactor * light.color * diffuseFactor * material.textures[textureIndex].diffuse.color;
	}
	else
	{
		diffuse = light.diffuseFactor * light.color * diffuseFactor * texture(material.textures[textureIndex].diffuse.tex, v_TexCoord);
	}

	// Specular
	vec3	viewDir				= normalize(viewPos - fragmentPosition);
	vec3	reflectDir			= reflect(-lightDir, norm);
	float	shininessFactor		= pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec4	specular;
	if (material.textures[textureIndex].specular.type == MaterialColor_Normal)
	{
		specular = light.specularFactor * light.color * shininessFactor * material.textures[textureIndex].specular.color;
	}
	else
	{
		specular = light.specularFactor * light.color * shininessFactor * texture(material.textures[textureIndex].specular.tex, v_TexCoord);
	}

	// Emission
	vec4	emission;
	if (material.textures[textureIndex].emission.type == MaterialColor_Normal)
	{
		emission = material.textures[textureIndex].emission.color;
	}
	else
	{
		emission = texture(material.textures[textureIndex].emission.tex, v_TexCoord);
	}

	// Attenuation factor
	float	distance			= length(light.position - fragmentPosition);
	float	attenuation_factor	= 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
									* light.attenuation_factor_quadratic * distance * distance);

	return ((ambient + diffuse + specular + emission) * attenuation_factor);
}

vec4 calculateSpotLightColor(Light light, int textureIndex)
{
	// Ambient
	vec4	ambient;
	if (material.textures[textureIndex].ambient.type == MaterialColor_Normal)
	{
		ambient = light.ambientFactor * light.color * material.textures[textureIndex].ambient.color;
	}
	else
	{
		ambient = light.ambientFactor * light.color * texture(material.textures[textureIndex].ambient.tex, v_TexCoord);
	}

	// Diffuse
	vec3	norm				= normalize(v_normalVec);
	vec3	lightDir			= normalize(light.position - fragmentPosition);
	float	diffuseFactor		= max(dot(norm, lightDir), 0.0f);
	vec4	diffuse;
	if (material.textures[textureIndex].diffuse.type == MaterialColor_Normal)
	{
		diffuse = light.diffuseFactor * light.color * diffuseFactor * material.textures[textureIndex].diffuse.color;
	}
	else
	{
		diffuse = light.diffuseFactor * light.color * diffuseFactor * texture(material.textures[textureIndex].diffuse.tex, v_TexCoord);
	}

	// Specular
	vec3	viewDir				= normalize(viewPos - fragmentPosition);
	vec3	reflectDir			= reflect(-lightDir, norm);
	float	shininessFactor		= pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec4	specular;
	if (material.textures[textureIndex].specular.type == MaterialColor_Normal)
	{
		specular = light.specularFactor * light.color * shininessFactor * material.textures[textureIndex].specular.color;
	}
	else
	{
		specular = light.specularFactor * light.color * shininessFactor * texture(material.textures[textureIndex].specular.tex, v_TexCoord);
	}

	// Emission
	vec4	emission;
	if (material.textures[textureIndex].emission.type == MaterialColor_Normal)
	{
		emission = material.textures[textureIndex].emission.color;
	}
	else
	{
		emission = texture(material.textures[textureIndex].emission.tex, v_TexCoord);
	}

	// Attenuation factor
	float	distance			= length(light.position - fragmentPosition);
	float	attenuation_factor	= 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
									* light.attenuation_factor_quadratic * distance * distance);

	// Spotlight
	float	intensity			= 1.0f;
	float	theta				= dot(lightDir, normalize(-light.direction));
	float	epsilon				= light.innerCutOffAngle - light.outerCutOffAngle;
			intensity			= clamp((theta - light.outerCutOffAngle) / epsilon, 0.0f, 1.0f);

	if (theta < light.outerCutOffAngle)
	{
		diffuse	 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		specular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return ((ambient + diffuse * intensity + specular * intensity + emission) * attenuation_factor);
}
