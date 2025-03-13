#pragma once

#include "ShaderComponent.h"
#include "SpriteComponent.h"
#include "DebugUIComponents.h"
#include "OpenGLBuffer.h"

namespace Hori
{
	struct ControllerComponent
	{
	};

	enum class DebugDraw
	{
		COLLIDER_WIREFRAME
	};

	struct DebugRendererComponent
	{
		void Switch(DebugDraw option)
		{
			enabled[option] = !enabled[option];
		}

		std::unordered_map<DebugDraw, bool> enabled{};
	};

	struct FPSComponent
	{
		float timeCount = 0.0f;
		int framesCount = 0;
		int fps = 0;
	};

	struct InputComponent
	{
		std::vector<int> input = std::vector<int>(1024);
	};

	struct TextComponent
	{
		std::string text{};
		glm::vec3 color{};
	};
	
	struct TransformComponent
	{
		glm::vec2 position{};
		float rotation{};
		glm::vec2 scale{};
	};

	struct VelocityComponent
	{
		glm::vec2 dir{};
		float speed{};
	};

	struct WireframeComponent
	{
		// Constructor
		WireframeComponent(std::vector<float>& vertices, glm::vec3 color)
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


}