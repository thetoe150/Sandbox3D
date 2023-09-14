#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.hpp"

#include <string>
#include <vector>
#include <iostream>

#define MAX_BONE_INFLUENCE 4

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int boneIDs[MAX_BONE_INFLUENCE];
	int weights[MAX_BONE_INFLUENCE];
};

struct Texture{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh{
public:
	std::vector<Vertex>		 m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture>		 m_textures;
	unsigned int m_VAO;

	// constructor
	// -------------------------------------------------------------------------
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
	  std::vector<Texture> textures)
	: m_vertices(vertices), m_indices(indices), m_textures(textures)
	{
		std::cout << "call mesh constructor " << std::endl;
		setupMesh();
	}

	// draw
	// -------------------------------------------------------------------------
	void draw(Shader &shader)
	{
		unsigned int diffuseNr  = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr   = 1;
		unsigned int heightNr   = 1;
		for(unsigned int i = 0; i < m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = m_textures[i].type;
			if(name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if(name == "texture_specular")
				number = std::to_string(specularNr++);	
			else if(name == "texture_normal")
				number = std::to_string(normalNr++);	
			else if(name == "texture_height")
				number = std::to_string(heightNr++);	

			glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str())
						, i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		}

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()),
						GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int m_VBO, m_EBO;

	// setup
	// -------------------------------------------------------------------------
	void setupMesh()
	{
		glGenVertexArrays(1, &m_VAO);	
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex)
			   , &m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int)
					, &m_indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex)
							  , (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex)
							  , (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex)
							  , (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex)
							  , (void*)offsetof(Vertex, tangent));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex)
							  , (void*)offsetof(Vertex, bitangent));
		
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex)
							  , (void*)offsetof(Vertex, boneIDs));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex)
							  , (void*)offsetof(Vertex, weights));
	}
};


