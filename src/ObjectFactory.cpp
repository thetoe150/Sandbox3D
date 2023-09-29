#include "ObjectFactory.hpp"
#include "VertexArrayGenerator.hpp"

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

VAO* ObjectFactory::makeVAO(vag::Object* object)
{
	const float* VertexData = object->getInterleavedVertices();
	unsigned int VertexSize = object->getInterleavedVertexSize();
	const unsigned int* IndexData = object->getIndices();
	unsigned int IndexSize = object->getIndexSize();
	unsigned int IndexCount = object->getIndexCount();

	VAO* vao = new VAO(VertexData, VertexSize, 3, 3, 2);
	vao->addEBO(IndexData, IndexSize, IndexCount);

	return vao;
};
vag::Object* ObjectFactory::makeVertexData(float p1, float p2, float p3)
{
	return nullptr;
};

vag::Object* SphereFactory::makeVertexData(float p1, float p2, float p3)
{
	vag::Sphere* sphere = new vag::Sphere(p1, (int)p2, (int)p3);

	return sphere;
};

vag::Object* CylinderFactory::makeVertexData(float p1, float p2, float p3)
{
	vag::Cylinder* cylinder = new vag::Cylinder(p1, p2, p3);

	return cylinder;
};

