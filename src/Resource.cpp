#include "Resource.hpp"

std::unordered_map<SHADERS, Shader> ShaderCollection{0};
std::unordered_map<TEXTURES, unsigned int> TextureCollection{0};

void LoadAllShader()
{
	Shader colorShader("D:/PP/Sandbox/src/shader/color_phong_vs.glsl", 
					"D:/PP/Sandbox/src/shader/color_phong_fs.glsl");
	Shader textureShader("D:/PP/Sandbox/src/shader/texture_phong_vs.glsl", 
					  "D:/PP/Sandbox/src/shader/texture_phong_fs.glsl");
	Shader lightSrcShader("D:/PP/Sandbox/src/shader/light_source_vs.glsl", 
					  "D:/PP/Sandbox/src/shader/light_source_fs.glsl");

	ShaderCollection[SHADERS::PHONG_3_LIGHT] = textureShader;
	ShaderCollection[SHADERS::COLOR_LIGHT] = colorShader;
	ShaderCollection[SHADERS::LIGHT_SOURCE] = lightSrcShader;
}

void LoadAllTexture()
{
	unsigned int boxDiffTex = loadTexture("D:/PP/Sandbox/res/box_diff.png");
	unsigned int boxSpecTex = loadTexture("D:/PP/Sandbox/res/box_spec.png");
	unsigned int boxEmitTex = loadTexture("D:/PP/Sandbox/res/box_emit.png");
	unsigned int basketTex = loadTexture("D:/PP/Sandbox/res/basket_ball.png");
	unsigned int saluteTex = loadTexture("D:/PP/Sandbox/res/saluting.png");
	unsigned int nerdTex = loadTexture("D:/PP/Sandbox/res/nerding.jpg");
	unsigned int moonTex = loadTexture("D:/PP/Sandbox/res/moon.png");

	TextureCollection[TEXTURES::BOX_DIFF] = boxDiffTex;
	TextureCollection[TEXTURES::BOX_SPEC] = boxSpecTex;
	TextureCollection[TEXTURES::BOX_EMIT] = boxEmitTex;
	TextureCollection[TEXTURES::BASKET_BALL] = basketTex;
	TextureCollection[TEXTURES::SALUTE] = saluteTex;
	TextureCollection[TEXTURES::NERDING] = nerdTex;
	TextureCollection[TEXTURES::MOON] = moonTex;
}

