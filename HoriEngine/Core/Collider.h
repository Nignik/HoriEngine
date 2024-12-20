#pragma once

#include <vector>

#include <Entity.h>
#include <glm/glm.hpp>

#include "Transform.h"
#include "EventManager.h"

namespace Hori
{
	struct SphereCollider
	{
		SphereCollider(Transform transform, bool isTrigger = false)
			: transform({ transform.position + transform.scale * 0.5f, transform.rotation, transform.scale * 0.5f }),
			radius(transform.scale.x * 0.5f),
			isTrigger(isTrigger)
		{
		}

		Transform transform;
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
