#version 430 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec2 aTexCoord; 

out vec3 vFragPos;
out vec2 vTexCoord;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main(){
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

	vFragPos = vec3(uModel * vec4(aPos, 1.0));
	vTexCoord = aTexCoord;
	vNormal = mat3(transpose(inverse(uModel))) * aNormal;
}
