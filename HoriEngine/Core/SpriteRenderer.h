#pragma once

#include <System.h>
#include <Entity.h>

#include "Sprite.h"
#include "Texture.h"
#include "OpenGLBuffer.h"
#include "Ecs.h"


namespace Hori
{
	class SpriteRenderer : public System
	{
	public:
		SpriteRenderer();

		void Update(float deltaTime) override;

	private:
		void DrawSprite(const Entity& entity);

		std::shared_ptr<OpenGLBuffer> m_quad;
	};
}