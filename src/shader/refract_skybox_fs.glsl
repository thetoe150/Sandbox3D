#version 430 core

in vec3 vPos;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform sampler3D skybox;

void main()
{
	float ratio = 1.00 / 1.52;
	vec3 v = normalize(vPos - cameraPos);
	vec3 refract = refract(v, normalize(vNormal), ratio);
	FragColor = vec4(texture(skybox, refract).rgb, 1.0); 
}
