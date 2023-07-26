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
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

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
	
private:
	void checkCompileErrors(GLuint shader, std::string type, const char* Path)
	{
		GLint success = -1;
		GLchar infoLog[1024];
		if(type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR from shader: " << 
				Path <<" of type: " << type << "\n" << infoLog << 
				"\n ---------------------------------------------" << std::endl;
			}
		}
		else
		{
			glGetShaderiv(shader, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_LINK_ERROR of type: " << 
				type << "\n" << infoLog << 
				"\n ---------------------------------------------" << std::endl;
			}
		}
	}
};

