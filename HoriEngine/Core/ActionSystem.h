#pragma once

#include <System.h>

#include <Entity.h>
#include "Ecs.h"

namespace Hori
{
	class ActionSystem : public System
	{
	public:
		ActionSystem() = default;

		void Update(float deltaTime) override;

	private:
		void Move(const Entity& entity, std::vector<int>& input);
	};
}

