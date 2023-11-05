#include "Resource.hpp"

std::unordered_map<SHADERS, Shader> g_shaderCollection{0};
std::unordered_map<FULL_SHADERS, FullShader> g_fullShaderCollection{0};
std::unordered_map<TEXTURES, unsigned int> g_textureCollection{0};
TexInfo g_terrainTexture;
unsigned int g_skyBoxTexture;

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
	Shader reflect("D:/PP/Sandbox/src/shader/reflect_skybox_vs.glsl", 
									  "D:/PP/Sandbox/src/shader/reflect_skybox_fs.glsl");
	Shader refract("D:/PP/Sandbox/src/shader/refract_skybox_vs.glsl", 
									  "D:/PP/Sandbox/src/shader/refract_skybox_fs.glsl");

	// Shader* terrainShader = new FullShader("D:/PP/Sandbox/src/shader/terrain_vs.glsl",
	// 						"D:/PP/Sandbox/src/shader/terrain_tcs.glsl",
	// 						"D:/PP/Sandbox/src/shader/terrain_tes.glsl",
	// 						 nullptr,
	// 						"D:/PP/Sandbox/src/shader/terrain_fs.glsl");

	g_shaderCollection[SHADERS::PHONG_3_LIGHT] = textureShader;
	g_shaderCollection[SHADERS::COLOR_LIGHT] = colorShader;
	g_shaderCollection[SHADERS::LIGHT_SOURCE] = lightSrcShader;
	g_shaderCollection[SHADERS::SKY_BOX] = skyboxShader;
	g_shaderCollection[SHADERS::REFLECT_SKYBOX] = reflect;
	g_shaderCollection[SHADERS::REFRACT_SKYBOX] = refract;

	
	// Terrain
	// ----------------------------------------------------------------------------------
	FullShader terrainShader("D:/PP/Sandbox/src/shader/terrain_vs.glsl",
							"D:/PP/Sandbox/src/shader/terrain_tcs.glsl",
							"D:/PP/Sandbox/src/shader/terrain_tes.glsl",
							 nullptr,
							"D:/PP/Sandbox/src/shader/terrain_fs.glsl");

	g_fullShaderCollection[FULL_SHADERS::TERRAIN] = terrainShader;
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

	g_textureCollection[TEXTURES::BOX_DIFF] = boxDiffTex;
	g_textureCollection[TEXTURES::BOX_SPEC] = boxSpecTex;
	g_textureCollection[TEXTURES::BOX_EMIT] = boxEmitTex;
	g_textureCollection[TEXTURES::BASKET_BALL] = basketTex;
	g_textureCollection[TEXTURES::SALUTE] = saluteTex;
	g_textureCollection[TEXTURES::NERDING] = nerdTex;
	g_textureCollection[TEXTURES::MOON] = moonTex;
	g_textureCollection[TEXTURES::TERRAIN] = terrainTex;

	g_terrainTexture = loadTexture("D:/PP/Sandbox/res/iceland_heightmap.png");

	std::array<const char*, 6> skyBoxTex =
	{
		"D:/PP/Sandbox/res/skybox/right.jpg",
		"D:/PP/Sandbox/res/skybox/left.jpg",
		"D:/PP/Sandbox/res/skybox/top.jpg",
		"D:/PP/Sandbox/res/skybox/bottom.jpg",
		"D:/PP/Sandbox/res/skybox/front.jpg",
		"D:/PP/Sandbox/res/skybox/back.jpg"
	};
	g_skyBoxTexture = loadCubeMap(skyBoxTex);
}
