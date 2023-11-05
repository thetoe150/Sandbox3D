#pragma once

#include <iostream>
#include <unordered_map>

#include "shader.hpp"
#include "texture.hpp"


void LoadAllShader();
void LoadAllTexture();

enum class SHADERS{
	PHONG_3_LIGHT,
	PHONG_1_LIGHT,
	COLOR_LIGHT,
	LIGHT_SOURCE,
	SKY_BOX,
	REFLECT_SKYBOX,
	REFRACT_SKYBOX
};

enum class FULL_SHADERS{
	TERRAIN
};

enum class TEXTURES{
	BOX_DIFF,
	BOX_SPEC,
	BOX_EMIT,
	BASKET_BALL,
	SALUTE,
	NERDING,
	MOON,
	TERRAIN
};

extern std::unordered_map<SHADERS, Shader> g_shaderCollection;
extern std::unordered_map<FULL_SHADERS, FullShader> g_fullShaderCollection;
extern std::unordered_map<TEXTURES, unsigned int> g_textureCollection;
extern TexInfo g_terrainTexture;
extern unsigned int g_skyBoxTexture;
