#pragma once

#include <vector>

#include <Entity.h>
#include <glm/glm.hpp>

#include "Components.h"
#include "EventManager.h"
#include "Ecs.h"

namespace Hori
{
	struct CollisionEvent : public IEvent
	{
		CollisionEvent(Entity entityA, Entity entityB, glm::vec2 contactPoint, glm::vec2 normal)
			: entityA{entityA},
			entityB{entityB},
			contactPoint{contactPoint},
			normal{normal}
		{
		}

		Entity entityA{};
		Entity entityB{};
		glm::vec2 contactPoint{};
		glm::vec2 normal{};
	};

	struct TriggerEvent : public IEvent
	{
		TriggerEvent(Entity entityA, Entity entityB)
			: entityA{ entityA },
			entityB{ entityB }
		{
		}

		Entity entityA{};
		Entity entityB{};
	};
}
