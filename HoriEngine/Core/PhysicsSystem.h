#pragma once

#include <Entity.h>
#include <System.h>
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

		bool BBCollision(Entity& entityA, Entity& entityB);
		bool SSCollision(Entity& entityA, Entity& entityB, float deltaTime);
		bool BSCollision(Entity& boxEntity, Entity& sphereEntity);
		void Move(Entity& entity, float deltaTime);
	};
}

