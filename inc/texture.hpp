#pragma once

#include <iostream>

#include "stb_image.hpp"
#include "glad/glad.h"

static unsigned int loadTexture(const char* path)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrComponent;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponent, 0);

	unsigned int texID{0};

	if(data)
	{
		GLenum format;
		if(nrComponent == 1)
			format = GL_RED;
		else if(nrComponent == 3)
			format = GL_RGB;
		else if(nrComponent == 4)
			format = GL_RGBA;

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, 
					GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		stbi_image_free(data);	
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Texture failed to load at path" << path <<std::endl;
		stbi_image_free(data);	
	}

	return texID;
}

