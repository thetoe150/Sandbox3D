#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath, std::fstream::in);
			fShaderFile.open(fragmentPath, std::fstream::in);

			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vShaderID, fShaderID;
		try
		{
			vShaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vShaderID, 1, &vShaderCode, NULL);
			glCompileShader(vShaderID);
			checkCompileErrors(vShaderID, "VERTEX", vertexPath);

			fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fShaderID, 1, &fShaderCode, NULL);
			glCompileShader(fShaderID);
			checkCompileErrors(fShaderID, "FRAGMENT", fragmentPath);

			this->ID = glCreateProgram();
			glAttachShader(this->ID, vShaderID);
			glAttachShader(this->ID, fShaderID);
			glLinkProgram(this->ID);
			checkCompileErrors(this->ID, "PROGRAM", NULL);
		}
		catch(std::exception& e)
		{
			std::cout << "ERROR::SHADER::COMPILE_SHADER_NOT_SUCCESSFULLY: " << e.what() << std::endl;
		}

		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);
	}
	
	// Constructor
	// ------------------------------------------------------------------------
	Shader() : ID(0){};
	
	Shader(const Shader &src)
	{
		this->ID = src.ID;
	}
	
	void use() const
	{
		glUseProgram(this->ID);
	}

	unsigned int getID() const
	{
		return this->ID;
	}

	// set uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setVec2(const std::string& name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}

	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); 
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); 
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
	
protected:
	void checkCompileErrors(GLuint object, std::string type, const char* Path)
	{
		GLint success = -1;
		GLchar infoLog[1024];
		if(type != "PROGRAM")
		{
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR from shader: " << 
				Path <<" of type: " << type << "\n" << infoLog << 
				"\n ---------------------------------------------" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_LINK_ERROR of type: " << 
				type << "\n" << infoLog << 
				"\n ---------------------------------------------" << std::endl;
			}
		}
	}
};

class ComputeShader : public Shader{
public:
	ComputeShader(const char* computePath)
	{
		std::string cShaderStr;
		std::ifstream cShaderFile;
		cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			cShaderFile.open(computePath, std::fstream::in);
			std::stringstream cShaderStream;
			cShaderStream << cShaderFile.rdbuf();
			cShaderFile.close();
			cShaderStr = cShaderStream.str();
		}
		catch(std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}

		const char* cShaderCode = cShaderStr.c_str();
		GLuint cShaderID;
		try
		{
			GLuint cShaderID = glCreateShader(GL_COMPUTE_SHADER);
			glShaderSource(cShaderID, 1, &cShaderCode, NULL);
			glCompileShader(cShaderID);
			checkCompileErrors(cShaderID, "FRAGMENT", computePath);

			this->ID = glCreateProgram();
			glAttachShader(this->ID, cShaderID);
			glLinkProgram(this->ID);
			checkCompileErrors(this->ID, "PROGRAM", NULL);
		}
		catch(std::exception& e)
		{
			std::cout << "ERROR::SHADER::COMPILE_SHADER_NOT_SUCCESSFULLY: " << e.what() << std::endl;
		}

		glDeleteShader(cShaderID);
	}

	void compute(unsigned int x, unsigned int y, unsigned int z) const
	{
		//use();
		glDispatchCompute(x, y, z);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
};

class FullShader : public Shader{
public:
	FullShader(){};
	FullShader(const char* vsPath, const char* tcsPath, const char* tesPath,
				const char* gsPath, const char* fsPath)
	{
		std::string vShaderStr;
		std::string tcShaderStr;
		std::string teShaderStr;
		std::string gShaderStr;
		std::string fShaderStr;
		std::ifstream vShaderFile;
		std::ifstream tcShaderFile;
		std::ifstream teShaderFile;
		std::ifstream gShaderFile;
		std::ifstream fShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// vertex shader
			vShaderFile.open(vsPath, std::fstream::in);
			std::stringstream vShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			vShaderFile.close();
			vShaderStr = vShaderStream.str();

			// tessellation shader
			if(tesPath != nullptr)
			{
				// tessellation control shader
				tcShaderFile.open(tcsPath, std::fstream::in);
				std::stringstream tcShaderStream;
				tcShaderStream << tcShaderFile.rdbuf();
				tcShaderFile.close();
				tcShaderStr = tcShaderStream.str();

				// tessellation evaluation shader
				teShaderFile.open(tesPath, std::fstream::in);
				std::stringstream teShaderStream;
				teShaderStream << teShaderFile.rdbuf();
				teShaderFile.close();
				teShaderStr = teShaderStream.str();
			}

			// geometry shader
			if(gsPath != nullptr)
			{
				gShaderFile.open(gsPath, std::fstream::in);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				gShaderStr = gShaderStream.str();
			}

			// fragment shader
			fShaderFile.open(fsPath, std::fstream::in);
			std::stringstream fShaderStream;
			fShaderStream << fShaderFile.rdbuf();
			fShaderFile.close();
			fShaderStr = fShaderStream.str();
		}
		catch(std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}

		GLuint vShaderID;
		GLuint tcShaderID;
		GLuint teShaderID;
		GLuint fShaderID;
		GLuint gShaderID;
		try
		{
			this->ID = glCreateProgram();

			vShaderID = glCreateShader(GL_VERTEX_SHADER);
			const char* vShaderCode = vShaderStr.c_str();
			glShaderSource(vShaderID, 1, &vShaderCode, NULL);
			glCompileShader(vShaderID);
			checkCompileErrors(vShaderID, "FRAGMENT", vsPath);
			glAttachShader(this->ID, vShaderID);

			if(tesPath != nullptr)
			{
				tcShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
				const char* tcShaderCode = tcShaderStr.c_str();
				glShaderSource(tcShaderID, 1, &tcShaderCode, NULL);
				glCompileShader(tcShaderID);
				checkCompileErrors(tcShaderID, "TESSELLATION_CONTROL", tcsPath);
				glAttachShader(this->ID, tcShaderID);

				teShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
				const char* teShaderCode = teShaderStr.c_str();
				glShaderSource(teShaderID, 1, &teShaderCode, NULL);
				glCompileShader(teShaderID);
				checkCompileErrors(teShaderID, "TESSELLATION_EVALUATION", tesPath);
				glAttachShader(this->ID, teShaderID);
			}

			if(gsPath != nullptr)
			{
				gShaderID = glCreateShader(GL_GEOMETRY_SHADER);
				const char* gShaderCode = gShaderStr.c_str();
				glShaderSource(gShaderID, 1, &gShaderCode, NULL);
				glCompileShader(gShaderID);
				checkCompileErrors(gShaderID, "GEOMETRY", gsPath);
				glAttachShader(this->ID, gShaderID);
			}

			fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fShaderCode = fShaderStr.c_str();
			glShaderSource(fShaderID, 1, &fShaderCode, NULL);
			glCompileShader(fShaderID);
			checkCompileErrors(fShaderID, "FRAGMENT", fsPath);
			glAttachShader(this->ID, fShaderID);

			glLinkProgram(this->ID);
			checkCompileErrors(this->ID, "PROGRAM", NULL);
		}
		catch(std::exception& e)
		{
			std::cout << "ERROR::SHADER::COMPILE_SHADER_NOT_SUCCESSFULLY: " << e.what() << std::endl;
		}

		glDeleteShader(vShaderID);
		glDeleteShader(tcShaderID);
		glDeleteShader(teShaderID);
		glDeleteShader(gShaderID);
		glDeleteShader(fShaderID);
	}

};
