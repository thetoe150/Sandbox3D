#include "Resource.hpp"

std::unordered_map<SHADERS, Shader> ShaderCollection{0};
std::unordered_map<FULL_SHADERS, FullShader> FullShaderCollection{0};
std::unordered_map<TEXTURES, unsigned int> TextureCollection{0};
TexInfo terrainTexture;
unsigned int skyBoxTexture;

void LoadAllShader()
{
	Shader colorShader("D:/PP/Sandbox/src/shader/color_phong_vs.glsl", 
									"D:/PP/Sandbox/src/shader/color_phong_fs.glsl");
	Shader textureShader("D:/PP/Sandbox/src/shader/texture_phong_vs.glsl", 
									  "D:/PP/Sandbox/src/shader/texture_phong_fs.glsl");
	Shader lightSrcShader("D:/PP/Sandbox/src/shader/light_source_vs.glsl", 
									  "D:/PP/Sandbox/src/shader/light_source_fs.glsl");
	Shader skyboxShader("D:/PP/Sandbox/src/shader/skybox_vs.glsl", 
									  "D:/PP/Sandbox/src/shader/skybox_fs.glsl");

	// Shader* terrainShader = new FullShader("D:/PP/Sandbox/src/shader/terrain_vs.glsl",
	// 						"D:/PP/Sandbox/src/shader/terrain_tcs.glsl",
	// 						"D:/PP/Sandbox/src/shader/terrain_tes.glsl",
	// 						 nullptr,
	// 						"D:/PP/Sandbox/src/shader/terrain_fs.glsl");

	ShaderCollection[SHADERS::PHONG_3_LIGHT] = textureShader;
	ShaderCollection[SHADERS::COLOR_LIGHT] = colorShader;
	ShaderCollection[SHADERS::LIGHT_SOURCE] = lightSrcShader;
	ShaderCollection[SHADERS::SKY_BOX] = skyboxShader;

	
	// Terrain
	// ----------------------------------------------------------------------------------
	FullShader terrainShader("D:/PP/Sandbox/src/shader/terrain_vs.glsl",
							"D:/PP/Sandbox/src/shader/terrain_tcs.glsl",
							"D:/PP/Sandbox/src/shader/terrain_tes.glsl",
							 nullptr,
							"D:/PP/Sandbox/src/shader/terrain_fs.glsl");

	FullShaderCollection[FULL_SHADERS::TERRAIN] = terrainShader;
}

void LoadAllTexture()
{
	unsigned int boxDiffTex = loadTexture("D:/PP/Sandbox/res/box_diff.png").texID;
	unsigned int boxSpecTex = loadTexture("D:/PP/Sandbox/res/box_spec.png").texID;
	unsigned int boxEmitTex = loadTexture("D:/PP/Sandbox/res/box_emit.png").texID;
	unsigned int basketTex = loadTexture("D:/PP/Sandbox/res/basket_ball.png").texID;
	unsigned int saluteTex = loadTexture("D:/PP/Sandbox/res/saluting.png").texID;
	unsigned int nerdTex = loadTexture("D:/PP/Sandbox/res/nerding.jpg").texID;
	unsigned int moonTex = loadTexture("D:/PP/Sandbox/res/moon.png").texID;
	unsigned int terrainTex = loadTexture("D:/PP/Sandbox/res/iceland_heightmap.png").texID;

	TextureCollection[TEXTURES::BOX_DIFF] = boxDiffTex;
	TextureCollection[TEXTURES::BOX_SPEC] = boxSpecTex;
	TextureCollection[TEXTURES::BOX_EMIT] = boxEmitTex;
	TextureCollection[TEXTURES::BASKET_BALL] = basketTex;
	TextureCollection[TEXTURES::SALUTE] = saluteTex;
	TextureCollection[TEXTURES::NERDING] = nerdTex;
	TextureCollection[TEXTURES::MOON] = moonTex;
	TextureCollection[TEXTURES::TERRAIN] = terrainTex;

	terrainTexture = loadTexture("D:/PP/Sandbox/res/iceland_heightmap.png");

	std::array<const char*, 6> skyBoxTex =
	{
		"D:/PP/Sandbox/res/skybox/right.jpg",
		"D:/PP/Sandbox/res/skybox/left.jpg",
		"D:/PP/Sandbox/res/skybox/bottom.jpg",
		"D:/PP/Sandbox/res/skybox/top.jpg",
		"D:/PP/Sandbox/res/skybox/front.jpg",
		"D:/PP/Sandbox/res/skybox/back.jpg"
	};
	skyBoxTexture = loadCubeMap(skyBoxTex);
}
