#version 430 core

in vec3 vPos;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	vec3 v = normalize(vPos - cameraPos);
	vec3 r = normalize(reflect(v, normalize(vNormal)));

	FragColor = vec4(texture(skybox, r).rgb, 1.0);
}

