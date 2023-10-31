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
};
extern std::unordered_map<SHADERS, Shader> ShaderCollection;

enum class FULL_SHADERS{
	TERRAIN
};
extern std::unordered_map<FULL_SHADERS, FullShader> FullShaderCollection;

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

extern std::unordered_map<TEXTURES, unsigned int> TextureCollection;
extern TexInfo terrainTexture;
