#pragma once

#include <Entity.h>
#include <System.h>
#include <Collider.h>
#include <unordered_set>
#include <unordered_map>

namespace Hori
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();

		void Update(float deltaTime) override;

	private:
		std::unordered_map<Entity, std::unordered_set<Entity>> m_activeTriggers{};
		std::unordered_map<Entity, std::unordered_set<Entity>> m_newTriggers{};

		bool CheckSphereSphereCollision(Entity eA, Entity eB);
		void ResolveSphereSphereCollision(Entity eA, Entity eB);
		void HandleTrigger(Entity eA, Entity eB, std::vector<TriggerEvent>& triggerEvents);
		void RemoveInactiveTriggers(Entity eA, Entity eB);
		void Move(Entity entity, float deltaTime);
	};
}

