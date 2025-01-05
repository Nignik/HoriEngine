#pragma once

#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "ResourceManager.h"
#include "OpenGLBuffer.h"

namespace Hori
{

	struct Buffer
	{
		GLuint vao{}, vbo{};

		~Buffer()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
		}
	};

	struct WireframeComponent
	{
		// Constructor
		WireframeComponent(std::vector<float>& vertices, glm::vec3 color)
			: buff(std::make_shared<Buffer>()),
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

		std::shared_ptr<Buffer> buff;
		int vertexCount{};
		glm::vec3 color{};
	};

}

