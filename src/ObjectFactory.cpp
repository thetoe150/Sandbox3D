#include "ObjectFactory.hpp"
#include "VertexArrayGenerator.hpp"
#include <algorithm>
#include <memory>

// constructor
// ----------------------------------------------------------------------------------------
SphereFactory::SphereFactory()
	: ObjectFactory()
{}

CylinderFactory::CylinderFactory()
	: ObjectFactory()
{}

LightSphereFactory::LightSphereFactory()
	: ObjectFactory()
{}

// make shader
// ----------------------------------------------------------------------------------------

Shader ObjectFactory::makeShader()
{
	return ShaderCollection[SHADERS::PHONG_3_LIGHT];
};

Shader SphereFactory::makeShader()
{
	return ShaderCollection[SHADERS::PHONG_3_LIGHT];
};

Shader CylinderFactory::makeShader()
{
	return ShaderCollection[SHADERS::PHONG_3_LIGHT];
};

Shader LightSphereFactory::makeShader()
{
	return ShaderCollection[SHADERS::LIGHT_SOURCE];
}

// make texture
// ----------------------------------------------------------------------------------------

unsigned int ObjectFactory::makeTexture(TEXTURES t)
{
	return TextureCollection[t];
};

// make VAO
// ----------------------------------------------------------------------------------------

std::unique_ptr<VAO> ObjectFactory::makeVAO(std::shared_ptr<vag::Object> object)
{
	const float* VertexData = object->getInterleavedVertices();
	unsigned int VertexSize = object->getInterleavedVertexSize();
	const unsigned int* IndexData = object->getIndices();
	unsigned int IndexSize = object->getIndexSize();
	unsigned int IndexCount = object->getIndexCount();

	// VAO* vao = new VAO(VertexData, VertexSize, 3, 3, 2);

	std::unique_ptr<VAO> vao = std::make_unique<VAO>(VertexData, VertexSize, 3, 3, 2);
	vao->addEBO(IndexData, IndexSize, IndexCount);

	return vao;
};

// make vertex data
// ----------------------------------------------------------------------------------------
std::shared_ptr<vag::Object> ObjectFactory::makeVertexData(float p1, float p2, float p3)
{
	return nullptr;
};

std::shared_ptr<vag::Object> SphereFactory::makeVertexData(float p1, float p2, float p3)
{
	auto sphere = std::make_shared<vag::Sphere>(p1, (int)p2, (int)p3);

	return sphere;
};

std::shared_ptr<vag::Object> CylinderFactory::makeVertexData(float p1, float p2, float p3)
{
	auto cylinder = std::make_shared<vag::Cylinder>(p1, p2, p3);

	return cylinder;
};

std::shared_ptr<vag::Object> LightSphereFactory::makeVertexData(float p1, float p2, float p3)
{
	auto sphere = std::make_shared<vag::Sphere>(p1, (int)p2, (int)p3);

	return sphere;
}

