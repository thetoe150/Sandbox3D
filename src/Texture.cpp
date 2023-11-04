#include "Texture.hpp"

TexInfo loadTexture(const char* path)
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
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);	

	return TexInfo{texID, width, height};
}

unsigned int loadCubeMap(std::array<const char*, 6> paths)
{
	stbi_set_flip_vertically_on_load(false);

	unsigned int cubeMapID;
	glGenTextures(1, &cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

	int width, height, nrComponent;
	for(unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(paths[i], &width, &height, &nrComponent, 0);
		if(data)
		{
			GLenum format;
			if(nrComponent == 1)
				format = GL_RED;
			else if(nrComponent == 3)
				format = GL_RGB;
			else if(nrComponent == 4)
				format = GL_RGBA;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height
				, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cube map failed to load at path: " << paths[i] << std::endl;
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return cubeMapID;
}

