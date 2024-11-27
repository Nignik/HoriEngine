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
		SpriteRenderer() = default;

		void Update(float deltaTime) override;

	private:
		void DrawSprite(Entity& entity);
	};
}