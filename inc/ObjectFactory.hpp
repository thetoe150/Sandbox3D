#pragma once

#include <iostream>

#include "SetupGlobalConstant.hpp"
#include "VertexArrayGenerator.hpp"
#include "VAO.hpp"

class ObjectFactory{
public:
	virtual VAO* makeVAO(float radius, int sector, int stack);
	Shader makeShaderRef(SHADERS s);
	unsigned int makeTextureRef(TEXTURES t);

protected:
	ObjectFactory();
};

class SphereFactory : public ObjectFactory{
public:
	SphereFactory();
	VAO* makeVAO(float radius, int sector, int stack) override;
};

void drawSphere(SphereFactory s);
