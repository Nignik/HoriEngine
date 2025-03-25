#pragma once

#include <vector>

#include <Entity.h>
#include <glm/glm.hpp>

#include "Components.h"
#include "EventManager.h"
#include "Ecs.h"

namespace Hori
{
	struct SphereCollider
	{
		SphereCollider() {};
		SphereCollider(TransformComponent transform, bool isTrigger = false)
			: transform(transform),
			radius(transform.scale.x * 0.5f),
			isTrigger(isTrigger)
		{
		}
		SphereCollider(TransformComponent transform, float radiusScale, bool isTrigger = false)
			: transform(transform),
			radius(transform.scale.x * radiusScale),
			isTrigger(isTrigger)
		{
		}

		TransformComponent transform{};
		float radius{};
		bool isTrigger = false;
	};

	struct CollisionEvent : public IEvent
	{
		CollisionEvent(Entity entityA, Entity entityB, glm::vec2 contactPoint, glm::vec2 normal)
			: entityA{entityA},
			entityB{entityB},
			contactPoint{contactPoint},
			normal{normal}
		{
		}

		Entity entityA;
		Entity entityB;
		glm::vec2 contactPoint;
		glm::vec2 normal;
	};

	struct TriggerEvent : public IEvent
	{
		TriggerEvent(Entity entityA, Entity entityB)
			: entityA{ entityA },
			entityB{ entityB }
		{
		}

		Entity entityA;
		Entity entityB;
	};
}
