#pragma once

#include "camera.hpp"
#include <vector>

// Constants 
// --------------------------------------------------------------------------------
extern const int WINDOW_HEIGHT;
extern const int WINDOW_WIDTH;

extern const float g_triangle[];
extern const unsigned int g_triangleLength;
extern const float g_colorBoxVertices[];
extern const unsigned int g_colorBoxVerticesLength;
extern const float g_textureBoxVertices[];
extern const unsigned int g_textureBoxVerticesLength;
extern const float g_skyBoxVertices[];
extern const unsigned int g_skyBoxVerticesLength;

// Variables 
// --------------------------------------------------------------------------------
extern float g_deltaTime;
extern float g_lastTime;

extern Camera g_camera;
extern glm::vec3 g_lightPositions;
