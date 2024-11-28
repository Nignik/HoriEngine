#pragma once

#include <System.h>
#include <Entity.h>

#include "Sprite.h"
#include "Texture.h"


namespace Hori
{
	class SpriteRenderer : public System
	{
	public:
		SpriteRenderer();

		void Update(float deltaTime) override;

	private:
		void DrawSprite(Entity& entity);

		GLuint m_quadVao;
	};
}