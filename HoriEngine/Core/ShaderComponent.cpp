#include "ShaderComponent.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Hori
{
	ShaderComponent& ShaderComponent::Use()
	{
		glUseProgram(this->Id);
		return *this;
	}

	void ShaderComponent::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
	{
		unsigned int sVertex, sFragment, gShader;
		// vertex Shader
		sVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(sVertex, 1, &vertexSource, NULL);
		glCompileShader(sVertex);
		checkCompileErrors(sVertex, "VERTEX");
		// fragment Shader
		sFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(sFragment, 1, &fragmentSource, NULL);
		glCompileShader(sFragment);
		checkCompileErrors(sFragment, "FRAGMENT");
		// if geometry shader source code is given, also compile geometry shader
		if (geometrySource != nullptr)
		{
			gShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShader, 1, &geometrySource, NULL);
			glCompileShader(gShader);
			checkCompileErrors(gShader, "GEOMETRY");
		}
		// shader program
		this->Id = glCreateProgram();
		glAttachShader(this->Id, sVertex);
		glAttachShader(this->Id, sFragment);
		if (geometrySource != nullptr)
			glAttachShader(this->Id, gShader);
		glLinkProgram(this->Id);
		checkCompileErrors(this->Id, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(sVertex);
		glDeleteShader(sFragment);
		if (geometrySource != nullptr)
			glDeleteShader(gShader);
	}

	void ShaderComponent::SetFloat(const char* name, float value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform1f(glGetUniformLocation(this->Id, name), value);
	}
	void ShaderComponent::SetInteger(const char* name, int value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform1i(glGetUniformLocation(this->Id, name), value);
	}
	void ShaderComponent::SetVector2f(const char* name, float x, float y, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform2f(glGetUniformLocation(this->Id, name), x, y);
	}
	void ShaderComponent::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform2f(glGetUniformLocation(this->Id, name), value.x, value.y);
	}
	void ShaderComponent::SetVector3f(const char* name, float x, float y, float z, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform3f(glGetUniformLocation(this->Id, name), x, y, z);
	}
	void ShaderComponent::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform3f(glGetUniformLocation(this->Id, name), value.x, value.y, value.z);
	}
	void ShaderComponent::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform4f(glGetUniformLocation(this->Id, name), x, y, z, w);
	}
	void ShaderComponent::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform4f(glGetUniformLocation(this->Id, name), value.x, value.y, value.z, value.w);
	}
	void ShaderComponent::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniformMatrix4fv(glGetUniformLocation(this->Id, name), 1, false, glm::value_ptr(matrix));
	}


	void ShaderComponent::checkCompileErrors(unsigned int object, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		}
		else
		{
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(object, 1024, NULL, infoLog);
				std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		}
	}
}

