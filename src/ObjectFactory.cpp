#include "ObjectFactory.hpp"

Shader ObjectFactory::makeShaderRef(SHADERS s)
{
	return ShaderCollection[s];
};

unsigned int ObjectFactory::makeTextureRef(TEXTURES t)
{
	return TextureCollection[t];
};

VAO* SphereFactory::makeVAO(float radius, int sector, int stack)
{
	vag::Sphere sphere(radius, sector, stack);
	const float* sphereVertexData = sphere.getInterleavedVertices();
	unsigned int sphereVertexSize = sphere.getInterleavedVertexSize();
	const unsigned int* sphereIndexData = sphere.getIndices();
	unsigned int sphereIndexSize = sphere.getIndexSize();
	unsigned int sphereIndexCount = sphere.getIndexCount();

	VAO* sphereVAO = new VAO(sphereVertexData, sphereVertexSize, 3, 3, 2);
	sphereVAO->addEBO(sphereIndexData, sphereIndexSize, sphereIndexCount);

	return sphereVAO;
};
