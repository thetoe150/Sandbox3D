#version 430 core

out vec4 FragColor;

in vec3 vFragPos;
in vec2 vTexCoord;
in vec3 vNormal;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float shininess;
	float brightness;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float Kc;
	float Kl;
	float Kq;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float Kc;
	float Kl;
	float Kq;
};


uniform Material uMaterial;
uniform DirLight uDirLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform vec3 uViewPos;

vec3 calcDirLight(DirLight light, vec3 n, vec3 fragPos, vec3 viewPos);
vec3 calcPointLight(PointLight light, vec3 n, vec3 fragPos, vec3 viewPos);
vec3 calcSpotLight(SpotLight light, vec3 n, vec3 fragPos, vec3 viewPos);
vec3 calcEmissionLight();

void main()
{
	vec3 n = normalize(vNormal);
	
	vec3 res = calcDirLight(uDirLight, n, vFragPos, uViewPos);
	res += calcPointLight(uPointLight, n, vFragPos, uViewPos);
	res += calcSpotLight(uSpotLight, n, vFragPos, uViewPos);
	//res += calcEmissionLight();

	FragColor = vec4(res, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 n, vec3 fragPos, vec3 viewPos)
{
	vec3 v = normalize(viewPos - fragPos);
	vec3 l = normalize(light.direction);
	vec3 r = reflect(-l, n);

	float diff = max(dot(n, l), 0.0);
	float spec = pow(max(dot(v, r), 0.0), uMaterial.shininess);

	vec3 ambient = light.ambient * texture(uMaterial.diffuse, vTexCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(uMaterial.diffuse, vTexCoord).rgb;
	vec3 specular = light.specular * spec * texture(uMaterial.specular, vTexCoord).rgb;
	vec3 combine = ambient + diffuse + specular;

	return combine;
}

vec3 calcPointLight(PointLight light, vec3 n, vec3 fragPos, vec3 viewPos)
{
	vec3 v = normalize(viewPos - fragPos);
	vec3 l = normalize(light.position - fragPos);
	vec3 r = reflect(-l, n);

	float diff = max(dot(n, l), 0.0);
	float spec = pow(max(dot(v, r), 0.0), uMaterial.shininess);

	vec3 ambient = light.ambient * texture(uMaterial.diffuse, vTexCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(uMaterial.diffuse, vTexCoord).rgb;
	vec3 specular = light.specular * spec * texture(uMaterial.specular, vTexCoord).rgb;
	vec3 combine = ambient + diffuse + specular;

	float distance = length(fragPos - light.position);
	float attenuation = 1 / (light.Kc + light.Kl * distance
						+ light.Kq * distance * distance);
	
	return combine * attenuation;
}

vec3 calcSpotLight(SpotLight light, vec3 n, vec3 fragPos, vec3 viewPos)
{
	vec3 lightDir = normalize(fragPos - light.position);	
	float theta = dot(lightDir, light.direction);
	float epsilon = light.cutoff - light.outerCutoff;
	float spotlight = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	if(spotlight == 0)
	{
		//vec3 ambient = light.ambient * texture(uMaterial.diffuse, vTexCoord).rgb;
		//return ambient * spotlight;
		// no ambient light when using flashlight
		return vec3(0.0);
	}
	else
	{
		vec3 v = normalize(viewPos - fragPos);
		vec3 l = normalize(light.position - fragPos);
		vec3 r = reflect(-l, n);

		float diff = max(dot(n, l), 0.0);
		float spec = pow(max(dot(v, r), 0.0), uMaterial.shininess);

		vec3 ambient = light.ambient * texture(uMaterial.diffuse, vTexCoord).rgb;
		vec3 diffuse = light.diffuse * diff * texture(uMaterial.diffuse, vTexCoord).rgb;
		vec3 specular = light.specular * spec * texture(uMaterial.specular, vTexCoord).rgb;
		vec3 combine = ambient + diffuse + specular;

		float distance = length(fragPos - light.position);
		float attenuation = 1 / (light.Kc + light.Kl * distance
							+ light.Kq * distance * distance);
		
		return combine * attenuation * spotlight;
	}
}

vec3 calcEmissionLight()
{
	vec3 emission = texture(uMaterial.emission, vTexCoord).rgb;
	return emission * uMaterial.brightness;
}
