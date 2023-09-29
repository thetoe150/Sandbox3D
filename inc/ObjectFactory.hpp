#pragma once

#include <iostream>

#include "Resource.hpp"
#include "VertexArrayGenerator.hpp"
#include "VAO.hpp"

class ObjectFactory{
public:
	virtual vag::Object* makeVertexData(float p1, float p2, float p3);
	VAO* makeVAO(vag::Object*);
	Shader makeShader(SHADERS s);
	unsigned int makeTexture(TEXTURES t);

protected:
	ObjectFactory(){};
};

class SphereFactory : public ObjectFactory{
public:
	SphereFactory();
	vag::Object* makeVertexData(float p1, float p2, float p3) override;
};

class CylinderFactory : public ObjectFactory{
public:
	CylinderFactory();
	vag::Object* makeVertexData(float p1, float p2, float p3) override;
};

