#include "ObjectFactory.hpp"
#include "Resource.hpp"
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
	return *ShaderCollection[SHADERS::PHONG_3_LIGHT];
};

Shader SphereFactory::makeShader()
{
	return *ShaderCollection[SHADERS::PHONG_3_LIGHT];
};

Shader CylinderFactory::makeShader()
{
	return *ShaderCollection[SHADERS::PHONG_3_LIGHT];
};

Shader LightSphereFactory::makeShader()
{
	return *ShaderCollection[SHADERS::LIGHT_SOURCE];
}

// make texture
// ----------------------------------------------------------------------------------------

std::vector<unsigned int> ObjectFactory::makeTexture(TEXTURES t)
{
	std::vector<unsigned int> res;
	if(t == TEXTURES::BOX_DIFF)
	{
		res.push_back(TextureCollection[TEXTURES::BOX_DIFF]);
		res.push_back(TextureCollection[TEXTURES::BOX_SPEC]);
		res.push_back(TextureCollection[TEXTURES::BOX_EMIT]);
	}
	else if(t == TEXTURES::SALUTE)
	{
		res.push_back(TextureCollection[TEXTURES::SALUTE]);
		res.push_back(TextureCollection[TEXTURES::BOX_SPEC]);
		res.push_back(TextureCollection[TEXTURES::BOX_EMIT]);
	}
	else if(t == TEXTURES::NERDING)
	{
		res.push_back(TextureCollection[TEXTURES::NERDING]);
		res.push_back(TextureCollection[TEXTURES::BOX_SPEC]);
		res.push_back(TextureCollection[TEXTURES::BOX_EMIT]);
	}
	else if(t == TEXTURES::MOON)
	{
		res.push_back(TextureCollection[TEXTURES::MOON]);
		res.push_back(TextureCollection[TEXTURES::BOX_SPEC]);
		res.push_back(TextureCollection[TEXTURES::BOX_EMIT]);
	}
	else
	{
		res.push_back(TextureCollection[TEXTURES::BASKET_BALL]);
		res.push_back(TextureCollection[TEXTURES::BOX_SPEC]);
		res.push_back(TextureCollection[TEXTURES::BOX_EMIT]);
	}
	return res;
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

