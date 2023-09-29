#include "ObjectFactory.hpp"
#include "VertexArrayGenerator.hpp"

VAO* ObjectFactory::makeVAO(float radius, float sector, float stack)
{
	return nullptr;
}

SphereFactory::SphereFactory()
	: ObjectFactory()
{}

CylinderFactory::CylinderFactory()
	: ObjectFactory()
{}

Shader ObjectFactory::makeShader(SHADERS s)
{
	return ShaderCollection[s];
};

unsigned int ObjectFactory::makeTexture(TEXTURES t)
{
	return TextureCollection[t];
};

VAO* SphereFactory::makeVAO(float p1, float p2, float p3)
{
	vag::Sphere sphere(p1, (int)p2, (int)p3);
	const float* sphereVertexData = sphere.getInterleavedVertices();
	unsigned int sphereVertexSize = sphere.getInterleavedVertexSize();
	const unsigned int* sphereIndexData = sphere.getIndices();
	unsigned int sphereIndexSize = sphere.getIndexSize();
	unsigned int sphereIndexCount = sphere.getIndexCount();

	VAO* sphereVAO = new VAO(sphereVertexData, sphereVertexSize, 3, 3, 2);
	sphereVAO->addEBO(sphereIndexData, sphereIndexSize, sphereIndexCount);

	return sphereVAO;
};

VAO* CylinderFactory::makeVAO(float p1, float p2, float p3)
{
	vag::Cylinder cylinder(p1, p2, p3);
	const float* cylinderVertexData = cylinder.getInterleavedVertices();
	unsigned int cylinderVertexSize = cylinder.getInterleavedVertexSize();
	const unsigned int* cylinderIndexData = cylinder.getIndices();
	unsigned int cylinderIndexSize = cylinder.getIndexSize();
	unsigned int cylinderIndexCount = cylinder.getIndexCount();

	VAO* cylinderVAO = new VAO(cylinderVertexData, cylinderVertexSize, 3, 3, 2);
	cylinderVAO->addEBO(cylinderIndexData, cylinderIndexSize, cylinderIndexCount);

	return cylinderVAO;
};

