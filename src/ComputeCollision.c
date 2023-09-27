#include "ComputeCollision.hpp"

float* reverseVec(const float* data, unsigned int length)
{
	float* res = new float[length]{0};
	GLuint ssboIn, ssboOut;
	glGenBuffers(1, &ssboIn);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIn);
	glBufferData(GL_SHADER_STORAGE_BUFFER, length * sizeof(float), data, GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboIn);

	glGenBuffers(1, &ssboOut);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);
	glBufferData(GL_SHADER_STORAGE_BUFFER, length * sizeof(float), res, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboIn);

	ComputeShader computeShader("D:/PP/Sandbox/src/shader/simple_cs.glsl");
	computeShader.compute(length / 64, 1, 1);

	float* read_data = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	for(unsigned int i = 0; i < length; i++)
	{
		std::cout << read_data[i] <<"\n";
	}
	res = read_data;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	return res;
}

void getOpenGLComputeLimit()
{
	int max_compute_work_group_count[3];
	int max_compute_work_group_size[3];
	int max_compute_work_group_invocations;

	for (int idx = 0; idx < 3; idx++) {
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &max_compute_work_group_count[idx]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &max_compute_work_group_size[idx]);
	}	
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_work_group_invocations);

	std::cout << "\nOpenGL Limitations: " << std::endl;
	std::cout << "maximum number of work groups in X dimension " << max_compute_work_group_count[0] << "\n";
	std::cout << "maximum number of work groups in Y dimension " << max_compute_work_group_count[1] << "\n";
	std::cout << "maximum number of work groups in Z dimension " << max_compute_work_group_count[2] << "\n";

	std::cout << "maximum size of a work group in X dimension " << max_compute_work_group_size[0] << "\n";
	std::cout << "maximum size of a work group in Y dimension " << max_compute_work_group_size[1] << "\n";
	std::cout << "maximum size of a work group in Z dimension " << max_compute_work_group_size[2] << "\n";

	std::cout << "Number of invocations in a single local work group that may be dispatched to a compute shader " << max_compute_work_group_invocations << std::endl;
}
