#pragma once

#include <iostream>
#include <memory>

#include "Resource.hpp"
#include "VertexArrayGenerator.hpp"
#include "VAO.hpp"

class ObjectFactory{
public:
	virtual std::shared_ptr<vag::Object> makeVertexData(float p1, float p2, float p3) = 0;
	virtual Shader						 makeShader();
	std::unique_ptr<VAO>				 makeVAO(std::shared_ptr<vag::Object>);
	std::vector<unsigned int>			 makeTexture(TEXTURES t);

protected:
	ObjectFactory(){};
};

class SphereFactory : public ObjectFactory{
public:
	SphereFactory();
	std::shared_ptr<vag::Object> makeVertexData(float p1, float p2, float p3) override;
	Shader						 makeShader() override;
};

class CylinderFactory : public ObjectFactory{
public:
	CylinderFactory();
	std::shared_ptr<vag::Object> makeVertexData(float p1, float p2, float p3) override;
	Shader						 makeShader() override;
};

class LightSphereFactory : public ObjectFactory{
public:
	LightSphereFactory();
	std::shared_ptr<vag::Object> makeVertexData(float p1, float p2, float p3) override;
	Shader						 makeShader() override;
};

// class TerrainBuilder{
// public:
// 	std::unique_ptr<VAO>  makeVAO();
// private:
// 	TextureData
// };
