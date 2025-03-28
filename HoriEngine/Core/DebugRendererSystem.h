#pragma once

#include <World.h>
#include <unordered_map>

#include "Components.h"
#include "Ecs.h"

namespace Hori
{
	class DebugRendererSystem : public System
	{
	public:
		DebugRendererSystem();
		void Update(float dt) override;

	private:
		void RenderWireframe(Entity entity);

		Shader m_shader{};
	};
}
