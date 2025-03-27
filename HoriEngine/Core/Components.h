#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

#include "DebugUIComponents.h"
#include "OpenGLBuffer.h"

namespace Hori
{
	struct Controller
	{
	};

	enum class DebugDraw
	{
		COLLIDER_WIREFRAME
	};

	struct DebugRenderer
	{
		void Switch(DebugDraw option)
		{
			enabled[option] = !enabled[option];
		}

		std::unordered_map<DebugDraw, bool> enabled{};
	};

	struct FramesCounter
	{
		float timeCount = 0.0f;
		int framesCount = 0;
		int fps = 0;
	};

	struct Input
	{
		std::vector<int> input = std::vector<int>(1024);
	};

	struct Text
	{
		std::string text{};
		glm::vec3 color{};
	};
	
	struct Transform
	{
		glm::vec2 position{};
		float rotation{};
		glm::vec2 scale{};
	};

	struct Velocity
	{
		glm::vec2 dir{};
		float speed{};
	};
	
	struct SphereCollider
	{
		SphereCollider() {};
		SphereCollider(Transform transform, bool isTrigger = false)
			: transform{std::move(transform)},
			radius{transform.scale.x * 0.5f},
			isTrigger{isTrigger}
		{
		}
		SphereCollider(Transform transform, float radiusScale, bool isTrigger = false)
			: transform{std::move(transform)},
			radius{transform.scale.x * radiusScale},
			isTrigger{isTrigger}
		{
		}

		Transform transform{};
		float radius{};
		bool isTrigger = false;
	};

	struct RectCollider
	{
		RectCollider() {};
		RectCollider(Transform transform, bool isTrigger = false)
			: transform{std::move(transform)},
			isTrigger{isTrigger}
		{
		}

		Transform transform{};
		bool isTrigger = false;
	};


	struct Wireframe
	{
		// Constructor
		Wireframe(std::vector<float>& vertices, glm::vec3 color)
			: buff(std::make_shared<OpenGLBuffer>()),
			color(color),
			vertexCount((int)vertices.size() / 2)
		{
			glCreateVertexArrays(1, &buff->vao);
			glCreateBuffers(1, &buff->vbo);

			glBindVertexArray(buff->vao);
			glBindBuffer(GL_ARRAY_BUFFER, buff->vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		std::shared_ptr<OpenGLBuffer> buff{};
		int vertexCount{};
		glm::vec3 color{};
	};

	struct Sprite
	{
		uint32_t Id{};
		uint32_t Width{}, Height{};
		uint32_t InternalFormat{};
		uint32_t imageFormat{};

		uint32_t WrapS{};
		uint32_t WrapT{};
		uint32_t FilterMin{};
		uint32_t FilterMax{};
		
		Sprite()
			: Width(0), Height(0), InternalFormat(GL_RGB), imageFormat(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
		{
			glGenTextures(1, &this->Id);
		}

		void Generate(uint32_t width, uint32_t height, unsigned char* data)
		{
			this->Width = width;
			this->Height = height;
			// create Texture
			glBindTexture(GL_TEXTURE_2D, this->Id);
			glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
			// set Texture wrap and filter modes
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
			// unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void Bind() const
		{
			glBindTexture(GL_TEXTURE_2D, this->Id);
		}
	};

	class Shader
	{
	public:
		unsigned int Id;
		Shader() {}

		void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr)
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

		void SetUniform(std::string_view name, const auto& value)
		{
			using T = std::remove_cvref_t<decltype(value)>;
			if constexpr (std::is_integral_v<T>)					glUniform1i(glGetUniformLocation(this->Id, name.data()), value);
			else if constexpr (std::is_floating_point_v<T>)			glUniform1f(glGetUniformLocation(this->Id, name.data()), value);
			else if constexpr (std::is_same_v<T, glm::vec2>)		glUniform2f(glGetUniformLocation(this->Id, name.data()), value.x, value.y);
			else if constexpr (std::is_same_v<T, glm::vec3>)		glUniform3f(glGetUniformLocation(this->Id, name.data()), value.x, value.y, value.z);
			else if constexpr (std::is_same_v<T, glm::vec4>)		glUniform4f(glGetUniformLocation(this->Id, name.data()), value.x, value.y, value.z, value.w);
			else if constexpr (std::is_same_v<T, glm::mat4>)		glUniformMatrix4fv(glGetUniformLocation(this->Id, name.data()), 1, false, glm::value_ptr(value));
			else													std::cout << "Error: invalid type passed to uniform " << name << '\n';
		}

		Shader& Use()
		{
			glUseProgram(this->Id);
			return *this;
		}
	private:
		void checkCompileErrors(unsigned int object, std::string type)
		{
			int success;
			char infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(object, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(object, 1024, NULL, infoLog);
					std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
				}
			}
			else
			{
				glGetProgramiv(object, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(object, 1024, NULL, infoLog);
					std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
				}
			}
		}
	};

	
}