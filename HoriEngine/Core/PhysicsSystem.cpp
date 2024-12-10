#include "PhysicsSystem.h"

#include "Collider.h"
#include "VelocityComponent.h"
#include "EventManager.h"

#include <World.h>
#include <iostream>

namespace Hori
{
	PhysicsSystem::PhysicsSystem()
	{

	}

	void PhysicsSystem::Update(float deltaTime)
	{
		World& world = World::GetInstance();

		for (auto entity : world.GetEntitiesWithComponents<VelocityComponent>())
		{
			if (!world.HasComponent<SphereCollider>(entity.GetID()))
			{
				Move(entity, deltaTime);
			}
		}

		for (auto entityA : world.GetEntitiesWithComponents<SphereCollider, VelocityComponent>())
		{
			bool isColliding = false;
			for (auto entityB : World::GetInstance().GetEntitiesWithComponents<SphereCollider>())
			{
				if (entityA.GetID() == entityB.GetID())
					continue;

				if (SSCollision(entityA, entityB, deltaTime))
				{
					isColliding = true;
					break;
				}

				// Check if the entityA trigger previously had entityB in inside, if so then delete it
				if (!isColliding && m_activeTriggers[entityA].find(entityB) != m_activeTriggers[entityA].end())
				{
					m_activeTriggers[entityA].erase(entityB);
				}
				if (!isColliding && m_activeTriggers[entityB].find(entityA) != m_activeTriggers[entityB].end())
				{
					m_activeTriggers[entityB].erase(entityA);
				}
			}

			if (!isColliding)
			{
				Move(entityA, deltaTime);
			}
		}
	}

	bool PhysicsSystem::BBCollision(Entity& entityA, Entity& entityB)
	{
		return false;
	}

	bool PhysicsSystem::SSCollision(Entity& entityA, Entity& entityB, float deltaTime)
	{
		auto& world = World::GetInstance();
		auto& colliderA = world.GetComponent<SphereCollider>(entityA);
		auto& colliderB = world.GetComponent<SphereCollider>(entityB);
		auto& colliderPosA = colliderA.transform.position;
		auto& colliderPosB = colliderB.transform.position;
		auto& objectPosA = world.GetComponent<Transform>(entityA).position;
		auto& objectPosB = world.GetComponent<Transform>(entityB).position;
		auto& rA = world.GetComponent<SphereCollider>(entityA).radius;
		auto& rB = world.GetComponent<SphereCollider>(entityB).radius;

		auto& vel1 = world.GetComponent<VelocityComponent>(entityA);

		if (glm::distance(colliderPosA + vel1.dir * vel1.speed * deltaTime, colliderPosB) <= rA + rB)
		{
			glm::vec2 dir = glm::normalize(colliderPosA - colliderPosB);
			glm::vec2 newPos = colliderPosB + dir * (rA + rB + 0.01f);
			glm::vec2 displacement = newPos - colliderPosA;

			if (colliderA.isTrigger)
			{
				if (m_activeTriggers[entityA].find(entityB) == m_activeTriggers[entityA].end())
				{
					m_activeTriggers[entityB].insert(entityB);
					std::cout << "Event triggered" << std::endl;
					TriggerEvent event(entityA, entityB);
					EventManager::GetInstance().AddEvents<TriggerEvent>(event);
				}
				
				Move(entityA, deltaTime);
			}
			else if (colliderB.isTrigger)
			{
				if (m_activeTriggers[entityB].find(entityA) == m_activeTriggers[entityB].end())
				{
					m_activeTriggers[entityB].insert(entityA);
					std::cout << "Event triggered" << std::endl;
					TriggerEvent event(entityB, entityA);
					EventManager::GetInstance().AddEvents<TriggerEvent>(event);
				}

				Move(entityA, deltaTime);
			}
			else
			{
				CollisionEvent event(entityA, entityB, dir * rB, dir);
				EventManager::GetInstance().AddEvents<CollisionEvent>(event);

				objectPosA += displacement;
				colliderPosA += displacement;
			}

			return true;
		}

		return false;
	}

	bool PhysicsSystem::BSCollision(Entity& boxEntity, Entity& sphereEntity)
	{
		return false;
	}

	void PhysicsSystem::Move(Entity& entity, float deltaTime)
	{
		auto& objectPos = World::GetInstance().GetComponent<Transform>(entity).position;
		auto& vel = World::GetInstance().GetComponent<VelocityComponent>(entity);

		if (World::GetInstance().HasComponent<SphereCollider>(entity.GetID()))
		{
			auto& colliderPos = World::GetInstance().GetComponent<SphereCollider>(entity).transform.position;
			colliderPos += vel.dir * vel.speed * deltaTime;
		}
		
		objectPos += vel.dir * vel.speed * deltaTime;
	}

}

