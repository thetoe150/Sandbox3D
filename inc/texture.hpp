#pragma once

#include <iostream>

#include "stb_image.hpp"
#include "glad/glad.h"
#include <tuple>

struct TexInfo{
	unsigned int texID;
	int width;
	int height;
};

TexInfo loadTexture(const char* path);
