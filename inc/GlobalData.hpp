#pragma once

#include "camera.hpp"

// Constants 
// --------------------------------------------------------------------------------
extern const int WINDOW_HEIGHT;
extern const int WINDOW_WIDTH;

extern const float triangle[];
extern const unsigned int triangleLength;
extern const float colorBoxVertices[];
extern const unsigned int colorBoxVerticesLength;
extern const float textureBoxVertices[];
extern const unsigned int textureBoxVerticesLength;

// Variables 
// --------------------------------------------------------------------------------
extern float deltaTime;
extern float lastTime;

extern Camera camera;
