#include "PhysicsSystem.h"

#include "Collider.h"
#include "VelocityComponent.h"

#include <World.h>


namespace Hori
{
	PhysicsSystem::PhysicsSystem()
	{

	}

	void PhysicsSystem::Update(float deltaTime)
	{
		World& world = World::GetInstance();

		for (const auto& entity1 : world.GetEntitiesWithComponents<BoxCollider, VelocityComponent>())
		{
			bool isColliding = false;
			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<BoxCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				BBCollision(entity1, entity2);
			}

			if (isColliding)
				continue;
			
			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<SphereCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				BSCollision(entity1, entity2);
			}
		}

		for (const auto& entity1 : world.GetEntitiesWithComponents<SphereCollider, VelocityComponent>())
		{
			bool isColliding = false;
			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<SphereCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				SSCollision(entity1, entity2, deltaTime);
			}

			if (isColliding)
				continue;

			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<BoxCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				BSCollision(entity2, entity1);
			}
		}
	}

	bool PhysicsSystem::BBCollision(const Entity& entity1, const Entity& entity2)
	{
		return false;
	}

	void PhysicsSystem::SSCollision(const Entity& entity1, const Entity& entity2, float deltaTime)
	{
		auto& colliderPos1 = World::GetInstance().GetComponent<SphereCollider>(entity1).transform.position;
		auto& colliderPos2 = World::GetInstance().GetComponent<SphereCollider>(entity2).transform.position;
		auto& objectPos1 = World::GetInstance().GetComponent<Transform>(entity1).position;
		auto& objectPos2 = World::GetInstance().GetComponent<Transform>(entity2).position;
		auto& r1 = World::GetInstance().GetComponent<SphereCollider>(entity1).radius;
		auto& r2 = World::GetInstance().GetComponent<SphereCollider>(entity2).radius;

		glm::vec2 vel1 = World::GetInstance().GetComponent<VelocityComponent>(entity1).velocity;

		if (glm::distance(colliderPos1 + vel1 * deltaTime, colliderPos2) <= r1 + r2)
		{
			glm::vec2 dir = glm::normalize(colliderPos1 - colliderPos2);
			glm::vec2 newPos = colliderPos2 + dir * (r1 + r2 + 0.01f);
			glm::vec2 displacement = newPos - colliderPos1;

			objectPos1 += displacement;
			colliderPos1 += displacement;
		}
		else
		{
			colliderPos1 += vel1 * deltaTime;
			objectPos1 += vel1 * deltaTime;
		}
	}

	bool PhysicsSystem::BSCollision(const Entity& boxEntity, const Entity& sphereEntity)
	{
		return false;
	}

}

