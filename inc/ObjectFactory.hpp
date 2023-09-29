#pragma once

#include <iostream>

#include "Resource.hpp"
#include "VertexArrayGenerator.hpp"
#include "VAO.hpp"

class ObjectFactory{
public:
	virtual VAO* makeVAO(float p1, float p2, float p3);
	Shader makeShader(SHADERS s);
	unsigned int makeTexture(TEXTURES t);

protected:
	ObjectFactory(){};
};

class SphereFactory : public ObjectFactory{
public:
	SphereFactory();
	VAO* makeVAO(float p1, float p2, float p3) override;
};

class CylinderFactory : public ObjectFactory{
public:
	CylinderFactory();
	VAO* makeVAO(float p1, float p2, float p3) override;
};

