#pragma once

#include <iostream>

#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "GLFW/glfw3.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "model.hpp"
#include "VAO.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include "VertexArrayGenerator.hpp"
#include "Collision.hpp"

#include "Resource.hpp"
#include "GlobalData.hpp"
#include "ObjectFactory.hpp"
#include "Object.hpp"

#include "Timer.hpp"

#include "Application.hpp"

//const int WINDOW_HEIGHT = 800;
//const int WINDOW_WIDTH = 1200;

//#include <windows.h>
//void printColorText()
//{
//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	// you can loop k higher to see more color choices
//	for(int k = 1; k < 255; k++)
//	{
//	// pick the colorattribute k you want
//	SetConsoleTextAttribute(hConsole, k);
//	std::cout << k << " I want to be nice today!" << std::endl;
//	}
//}
