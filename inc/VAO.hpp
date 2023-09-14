#pragma once

#include <iostream>
#include <vector>
#include "glad/glad.h"

class VAO
{
private:
	unsigned int vaoID;
	unsigned int eboID;
	std::vector<unsigned int> vboIDs;
	unsigned int vertexAttrCounter;	

public:
	// constructor
	//--------------------------------------------------------------------------
	VAO() : vaoID(0), eboID(0), vertexAttrCounter(0){}

	VAO(const float* data, unsigned int dataSize, unsigned int firstAttrLen, 
		unsigned int secondAttrLen, unsigned int thirdAttrLen,
		unsigned int divisor1 = 0, unsigned int divisor2 = 0,
		unsigned int divisor3 = 0)
		: vaoID(0), eboID(0), vertexAttrCounter(0)
	{
		glGenVertexArrays(1, &vaoID);
		addVBO(data, dataSize, firstAttrLen, secondAttrLen, thirdAttrLen, 
				divisor1, divisor2, divisor3);
	}

	VAO(const VAO& other) = delete;
    //{
    //    std::cout <<"Calling VAO copy constructor."<<std::endl;
    //    this->vaoID = other.vaoID;
    //    this->eboID = other.eboID;
    //    this->vboIDs = other.vboIDs;
    //    this->vertexAttrCounter = other.vertexAttrCounter;
    //}

    VAO& operator=(const VAO& other) = delete;
    //{
    //    std::cout <<"Calling VAO copy assignment."<<std::endl;
    //    this->vaoID = other.vaoID;
    //    this->eboID = other.eboID;
    //    this->vboIDs = other.vboIDs;
    //    this->vertexAttrCounter = other.vertexAttrCounter;

    //    return *this;
    //}

    VAO(VAO&& other) = delete;
    //{
    //    std::cout <<"Calling VAO move constructor."<<std::endl;
    //    this->vaoID = other.vaoID;
    //    this->eboID = other.eboID;
    //    this->vboIDs = std::move(other.vboIDs);
    //    this->vertexAttrCounter = other.vertexAttrCounter;
    //}

    VAO& operator=(VAO&& other) = delete;
    //{
    //    std::cout <<"Calling VAO move assignment."<<std::endl;
    //    this->vaoID = other.vaoID;
    //    this->eboID = other.eboID;
    //    this->vboIDs = std::move(other.vboIDs);
    //    this->vertexAttrCounter = other.vertexAttrCounter;

    //    return *this;
    //}

    ~VAO()
    {
        std::cout << "Calling VAO destructor" <<std::endl;
        glDeleteVertexArrays(1, &vaoID);
        glDeleteBuffers(1, &eboID);
        for(auto vbo : vboIDs)
        {
			glDeleteBuffers(1, &vbo);
        }
    }

    unsigned int getVAOID() const
    {
        return vaoID;
    }

	// add function
	//--------------------------------------------------------------------------
	unsigned int addVBO(const float* data, unsigned int dataSize,
						unsigned int firstAttrLen, unsigned int secondAttrLen, 
						unsigned int thirdAttrLen,
						unsigned int divisor1 = 0, unsigned int divisor2 = 0,
						unsigned int divisor3 = 0)
	{
		this->bind();
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		this->vboIDs.push_back(vboID);

		unsigned int totalAttrLen = firstAttrLen + secondAttrLen + thirdAttrLen;

		glEnableVertexAttribArray(vertexAttrCounter);
		glVertexAttribPointer(vertexAttrCounter, firstAttrLen, GL_FLOAT, GL_FALSE,
							  totalAttrLen * sizeof(float), (void*)0);
		glVertexAttribDivisor(vertexAttrCounter, divisor1);
		vertexAttrCounter++;

		if(secondAttrLen != 0)
		{
			glEnableVertexAttribArray(vertexAttrCounter);
			glVertexAttribPointer(vertexAttrCounter, secondAttrLen, GL_FLOAT, 
								 GL_FALSE, totalAttrLen * sizeof(float), 
								 (void*)(firstAttrLen * sizeof(float)));
			glVertexAttribDivisor(vertexAttrCounter, divisor2);
			vertexAttrCounter++;
		}

		if(thirdAttrLen != 0)
		{
			glEnableVertexAttribArray(vertexAttrCounter);
			glVertexAttribPointer(vertexAttrCounter, thirdAttrLen, GL_FLOAT, 
								 GL_FALSE, totalAttrLen * sizeof(float), 
								 (void*)((firstAttrLen + secondAttrLen) * sizeof(float)));
			glVertexAttribDivisor(vertexAttrCounter, divisor3);
			vertexAttrCounter++;
		}
		glBindVertexArray(0);

		return vertexAttrCounter - 1;
	}
	
	void addEBO(const unsigned int* data, unsigned int dataSize)
	{
		this->bind();
		glGenBuffers(1, &eboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, (void*)data, GL_STATIC_DRAW);
		this->unbind();
	}
	
	void bind()
	{
		glBindVertexArray(this->vaoID);
	}

	void unbind()
	{
		glBindVertexArray(0);
	}

    void print()
    {
        std::cout<<"VAO ID: "<< this->vaoID <<std::endl;
        std::cout<<"VBO ID: "<< this->vboIDs[0] <<std::endl;
        std::cout<<"EBO ID: "<< this->eboID <<std::endl;
    }
	
};
