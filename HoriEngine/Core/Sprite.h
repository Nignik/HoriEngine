#pragma once

#include <glad/glad.h>
#include <Component.h>
#include <memory>

#include "Transform.h"
#include "Shader.h"
#include "Texture.h"

namespace Hori
{
	struct Sprite
	{
	public:
		Sprite();
		~Sprite();

		void Draw();

	protected:
		friend class SpriteRenderer;

		GLuint m_vao, m_vbo;
	};
}