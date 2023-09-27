#version 430 core

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding = 0) readonly buffer inputNormal
{
	float inNormal[];
};

layout (std430, binding = 1) writeonly buffer outputNormal
{
	float outNormal[];
};

void main()
{
	uint i = gl_GlobalInvocationID.x;
	outNormal[i] = inNormal[i];
}
