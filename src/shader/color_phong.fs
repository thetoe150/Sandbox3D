#version 330 core

out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;
in vec3 vColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material uMaterial;
uniform Light uLight;
uniform vec3 viewPos;

void main(){
	vec3 n = normalize(vNormal);
	vec3 l = normalize(uLight.position - vFragPos);
	float diff = max(dot(l, n), 0.0);

	vec3 v = normalize(viewPos - vFragPos);
	vec3 r = reflect(-l, vNormal);	
	float spec = pow(max(dot(v, r), 0.0), uMaterial.shininess);

	vec3 ambient = uLight.ambient * uMaterial.ambient;
	vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);
	vec3 specular = uLight.specular * (spec * uMaterial.specular);

	vec3 phong = ambient + diffuse + specular;
	FragColor = vec4(phong * vColor, 1.0);
}
