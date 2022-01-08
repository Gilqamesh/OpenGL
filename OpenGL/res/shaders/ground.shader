#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normalVec;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 normalMatrix;

out vec3 v_normalVec;
out vec3 fragmentPosition;
out vec2 v_TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	fragmentPosition = vec3(model * vec4(position, 1.0f));
	v_normalVec = mat3(normalMatrix) * normalVec;
	// Only if Loaded from texture
	v_TexCoord = vec2(1.0f, 1.0f);
}

#shader fragment
#version 330 core
#define LightType_Point 0
#define LightType_Directional 1
#define LightType_Spot 2
#define MaterialColor_Tex 0
#define MaterialColor_Normal 1
#define N_OF_LIGHTS 1

layout(location = 0) out vec4 color;

in vec3 fragmentPosition;
in vec3 v_normalVec;
in vec2 v_TexCoord;

struct Material
{
	int colorType;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 emissionColor;
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

uniform vec3 viewPos;
uniform Light light[N_OF_LIGHTS];
uniform Material material;

vec3 calculatePointLightColor_Texture(Light light);
vec3 calculateDirectionalLightColor_Texture(Light light);
vec3 calculateSpotLightColor_Texture(Light light);

vec3 calculatePointLightColor_Normal(Light light);
vec3 calculateDirectionalLightColor_Normal(Light light);
vec3 calculateSpotLightColor_Normal(Light light);

void main()
{
	vec3 sumColor(0.0f, 0.0f, 0.0f);

	if (material.colorType == MaterialColor_Tex)
	{
		for (int i = 0; i < N_OF_LIGHTS; ++i)
		{
			if (light[i].type == LightType_Point)
				sumColor += calculatePointLightColor_Texture(light[i]);
			else if (light[i].type == LightType_Directional)
				sumColor += calculateDirectionalLightColor_Texture(light[i]);
			else if (light[i].type == LightType_Spot)
				sumColor += calculateSpotLightColor_Texture(light[i]);
		}
	}
	else if (material.colorType == MaterialColor_Normal)
	{
		for (int i = 0; i < N_OF_LIGHTS; ++i)
		{
			if (light[i].type == LightType_Point)
				sumColor += calculatePointLightColor_Normal(light[i]);
			else if (light[i].type == LightType_Directional)
				sumColor += calculateDirectionalLightColor_Normal(light[i]);
			else if (light[i].type == LightType_Spot)
				sumColor += calculateSpotLightColor_Normal(light[i]);
		}
	}
	color = vec4(sumColor, 1.0f);
}

vec3 calculatePointLightColor_Texture(Light light)
{
	// Ambient
	ambient = light.ambientFactor * light.color * texture(material.diffuseMap, v_TexCoord).rgb;

	// Diffuse
	vec3	norm			= normalize(v_normalVec);
	vec3	lightDir		= normalize(light.position - fragmentPosition);
	float	diffuseFactor	= max(dot(norm, lightDir), 0.0f);
	vec3	diffuse			= light.diffuseFactor * light.color * diffuseFactor * texture(material.diffuseMap, v_TexCoord).rgb;

	// Specular
	vec3	viewDir			= normalize(viewPos - fragmentPosition);
	vec3	reflectDir		= reflect(-lightDir, norm);
	float	specularFactor	= pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
	vec3	specular		= light.specularFactor * light.color * specularFactor * texture(
								(material.specularMap != -1 ? material.specularMap : material.diffuseMap), v_TexCoord).rgb;

	// Emission
	vec3	emission		= (material.emissionMap != -1 ? texture(material.emissionMap, v_TexCoord).rgb : vec3(0.0f, 0.0f, 0.0f));

	// Attenuation factor
	float	distance			= length(light.position - fragmentPosition);
	float	attenuation_factor	= 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
								  * light.attenuation_factor_quadratic * distance * distance);

	return ((ambient + diffuse + specular + emission) * attenuation_factor);
}

vec3 calculateDirectionalLightColor_Texture(Light light)
{
	// Ambient
	ambient = light.ambientFactor * light.color * texture(material.diffuseMap, v_TexCoord).rgb;

	// Diffuse
	vec3	norm = normalize(v_normalVec);
	vec3	lightDir = normalize(-light.direction);
	float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
	vec3	diffuse = light.diffuseFactor * light.color * diffuseFactor * texture(material.diffuseMap, v_TexCoord).rgb;

	// Specular
	vec3	viewDir = normalize(viewPos - fragmentPosition);
	vec3	reflectDir = reflect(-lightDir, norm);
	float	specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
	vec3	specular = light.specularFactor * light.color * specularFactor * texture(
		(material.specularMap != -1 ? material.specularMap : material.diffuseMap), v_TexCoord).rgb;

	// Emission
	vec3	emission = (material.emissionMap != -1 ? texture(material.emissionMap, v_TexCoord).rgb : vec3(0.0f, 0.0f, 0.0f));

	// Attenuation factor
	float	distance = length(light.position - fragmentPosition);
	float	attenuation_factor = 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
		* light.attenuation_factor_quadratic * distance * distance);

	return ((ambient + diffuse + specular + emission) * attenuation_factor);
}

vec3 calculateSpotLightColor_Texture(Light light)
{
	// Ambient
	ambient = light.ambientFactor * light.color * texture(material.diffuseMap, v_TexCoord).rgb;

	// Diffuse
	vec3	norm = normalize(v_normalVec);
	vec3	lightDir = normalize(light.position - fragmentPosition);
	float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
	vec3	diffuse = light.diffuseFactor * light.color * diffuseFactor * texture(material.diffuseMap, v_TexCoord).rgb;

	// Specular
	vec3	viewDir = normalize(viewPos - fragmentPosition);
	vec3	reflectDir = reflect(-lightDir, norm);
	float	specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
	vec3	specular = light.specularFactor * light.color * specularFactor * texture(
		(material.specularMap != -1 ? material.specularMap : material.diffuseMap), v_TexCoord).rgb;

	// Emission
	vec3	emission = (material.emissionMap != -1 ? texture(material.emissionMap, v_TexCoord).rgb : vec3(0.0f, 0.0f, 0.0f));

	// Attenuation factor
	float	distance			= length(light.position - fragmentPosition);
	float	attenuation_factor	= 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
								  * light.attenuation_factor_quadratic * distance * distance);

	// Spotlight
	float intensity = 1.0f;
	if (light.type == LightType_SpotLight)
	{
		float theta		= dot(lightDir, normalize(-light.direction));
		float epsilon	= light.innerCutOffAngle - light.outerCutOffAngle;
			  intensity	= clamp((theta - light.outerCutOffAngle) / epsilon, 0.0f, 1.0f);

		if (theta < light.outerCutOffAngle)
		{
			diffuse		= vec3(0.0f, 0.0f, 0.0f);
			specular	= vec3(0.0f, 0.0f, 0.0f);
		}
	}

	return ((ambient + diffuse * intensity + specular * intensity + emission) * attenuation_factor);
}

vec3 calculatePointLightColor_Normal(Light light)
{
	// Ambient
	ambient = light.ambientFactor * light.color * material.ambientColor;

	// Diffuse
	vec3	norm = normalize(v_normalVec);
	vec3	lightDir = normalize(light.position - fragmentPosition);
	float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
	vec3	diffuse = light.diffuseFactor * light.color * diffuseFactor * material.diffuseColor;

	// Specular
	vec3	viewDir = normalize(viewPos - fragmentPosition);
	vec3	reflectDir = reflect(-lightDir, norm);
	float	specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
	vec3	specular = light.specularFactor * light.color * specularFactor * material.specularColor;

	// Emission
	vec3	emission = material.emissionColor;

	// Attenuation factor
	float	distance		   = length(light.position - fragmentPosition);
	float	attenuation_factor = 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
								 * light.attenuation_factor_quadratic * distance * distance);

	return ((ambient + diffuse + specular + emission) * attenuation_factor);
}

vec3 calculateDirectionalLightColor_Normal(Light light)
{
	// Ambient
	ambient = light.ambientFactor * light.color * texture(material.diffuseMap, v_TexCoord).rgb;

	// Diffuse
	vec3	norm = normalize(v_normalVec);
	vec3	lightDir = normalize(-light.direction);
	float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
	vec3	diffuse = light.diffuseFactor * light.color * diffuseFactor * texture(material.diffuseMap, v_TexCoord).rgb;

	// Specular
	vec3	viewDir = normalize(viewPos - fragmentPosition);
	vec3	reflectDir = reflect(-lightDir, norm);
	float	specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
	vec3	specular = light.specularFactor * light.color * specularFactor * texture(
		(material.specularMap != -1 ? material.specularMap : material.diffuseMap), v_TexCoord).rgb;

	// Emission
	vec3	emission = material.emissionColor;

	// Attenuation factor
	float	distance = length(light.position - fragmentPosition);
	float	attenuation_factor = 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
		* light.attenuation_factor_quadratic * distance * distance);

	return ((ambient + diffuse + specular + emission) * attenuation_factor);
}

vec3 calculateSpotLightColor_Normal(Light light)
{
	// Ambient
	ambient = light.ambientFactor * light.color * material.ambientColor;

	// Diffuse
	vec3	norm = normalize(v_normalVec);
	vec3	lightDir = normalize(light.position - fragmentPosition);
	float	diffuseFactor = max(dot(norm, lightDir), 0.0f);
	vec3	diffuse = light.diffuseFactor * light.color * diffuseFactor * material.diffuseColor;

	// Specular
	vec3	viewDir = normalize(viewPos - fragmentPosition);
	vec3	reflectDir = reflect(-lightDir, norm);
	float	specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininessFactor);
	vec3	specular = light.specularFactor * light.color * specularFactor * material.specularColor;

	// Emission
	vec3	emission = material.emissionColor;

	// Attenuation factor
	float	distance			= length(light.position - fragmentPosition);
	float	attenuation_factor	= 1.0f / (light.attenuation_factor_constant + light.attenuation_factor_linear * distance
								  * light.attenuation_factor_quadratic * distance * distance);
	// Spotlight
	float intensity = 1.0f;
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

	return ((ambient + diffuse * intensity + specular * intensity + emission) * attenuation_factor);
}
