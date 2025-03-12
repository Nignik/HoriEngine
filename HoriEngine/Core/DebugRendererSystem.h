#pragma once

#include <World.h>
#include <unordered_map>

#include "WireframeComponent.h"
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

		std::shared_ptr<Shader> m_shader{};
	};
}
