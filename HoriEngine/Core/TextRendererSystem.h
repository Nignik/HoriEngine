#pragma once

#include <World.h>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "OpenGLBuffer.h"

namespace Hori
{
	struct Character
	{
		unsigned int textureID{};
		glm::ivec2   size{};
		glm::ivec2   bearing{};
		unsigned int advance{};
	};

	class TextRendererSystem : public System
	{
	public:
		TextRendererSystem();
		void Update(float dt) override;

	private:
		std::unordered_map<GLchar, Character> Characters{};
		Shader m_shader;
		std::shared_ptr<OpenGLBuffer> m_buffer{};

		void InitializeFreeType();
		void InitializeQuad();
		void RenderText(Entity entity);
	};
}