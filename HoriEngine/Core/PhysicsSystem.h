#pragma once

#include <Entity.h>
#include <System.h>

namespace Hori
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();

		void Update(float deltaTime) override;

	private:
		bool BBCollision(Entity& entityA, Entity& entityB);
		bool SSCollision(Entity& entityA, Entity& entityB, float deltaTime);
		bool BSCollision(Entity& boxEntity, Entity& sphereEntity);
		void Move(Entity& entity, float deltaTime);
	};
}