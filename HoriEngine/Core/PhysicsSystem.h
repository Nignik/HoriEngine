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
		bool BBCollision(const Entity& entity1, const Entity& entity2);
		void SSCollision(const Entity& entity1, const Entity& entity2, float deltaTime);
		bool BSCollision(const Entity& boxEntity, const Entity& sphereEntity);
	};
}