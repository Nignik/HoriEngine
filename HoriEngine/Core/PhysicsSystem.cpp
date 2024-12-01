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

		auto move = [&](Entity entity) {
			if (world.HasComponent<VelocityComponent>(entity.GetID()))
			{
				glm::vec2* colliderPosition;
				if (world.HasComponent<BoxCollider>(entity.GetID()))
					colliderPosition = &world.GetComponent<BoxCollider>(entity).transform.position;
				else
					colliderPosition = &world.GetComponent<SphereCollider>(entity).transform.position;

				auto& position = world.GetComponent<Transform>(entity).position;
				auto& velocity = world.GetComponent<VelocityComponent>(entity).velocity;

				position += velocity * deltaTime;
				*colliderPosition += velocity * deltaTime;
			}
			};

		for (const auto& entity1 : world.GetEntitiesWithComponents<BoxCollider>())
		{
			bool isColliding = false;
			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<BoxCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				if (BBCollision(entity1, entity2))
				{
					isColliding = true;
					break;
				}
			}

			if (isColliding)
				continue;
			
			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<SphereCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				if (BSCollision(entity1, entity2))
				{
					isColliding = true;
					break;
				}
			}

			if (!isColliding)
				move(entity1);
		}

		for (const auto& entity1 : world.GetEntitiesWithComponents<SphereCollider>())
		{
			bool isColliding = false;
			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<SphereCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				if (SSCollision(entity1, entity2, deltaTime))
				{
					isColliding = true;
					break;
				}
			}

			if (isColliding)
				continue;

			for (const auto& entity2 : World::GetInstance().GetEntitiesWithComponents<BoxCollider>())
			{
				if (entity1.GetID() == entity2.GetID())
					continue;

				if (BSCollision(entity2, entity1))
				{
					isColliding = true;
					break;
				}
			}

			if (!isColliding)
				move(entity1);
		}
	}

	bool PhysicsSystem::BBCollision(const Entity& entity1, const Entity& entity2)
	{
		return false;
	}

	bool PhysicsSystem::SSCollision(const Entity& entity1, const Entity& entity2, float deltaTime)
	{
		auto& pos1 = World::GetInstance().GetComponent<SphereCollider>(entity1).transform.position;
		auto& pos2 = World::GetInstance().GetComponent<SphereCollider>(entity2).transform.position;
		auto& r1 = World::GetInstance().GetComponent<SphereCollider>(entity1).radius;
		auto& r2 = World::GetInstance().GetComponent<SphereCollider>(entity2).radius;

		glm::vec2 vel1{};
		if (World::GetInstance().HasComponent<VelocityComponent>(entity1.GetID()))
			vel1 = World::GetInstance().GetComponent<VelocityComponent>(entity1).velocity;

		if (glm::distance(pos1 + vel1 * deltaTime, pos2) <= r1 + r2)
			return true;

		return false;
	}

	bool PhysicsSystem::BSCollision(const Entity& boxEntity, const Entity& sphereEntity)
	{
		return false;
	}

}

