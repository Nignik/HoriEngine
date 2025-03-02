#include "PhysicsSystem.h"

#include "Collider.h"
#include "VelocityComponent.h"
#include "EventManager.h"
#include "Renderer.h"

#include <World.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Rect.h"

namespace Hori
{
	struct CollisionPair
	{
		Entity entityA{};
		Entity entityB{};
	};

	PhysicsSystem::PhysicsSystem()
	{
	}

	void PhysicsSystem::Update(float deltaTime)
	{
		World& world = Ecs::GetInstance();

		std::vector<Entity> dynamicCollidables;
		for (auto entity : world.GetEntitiesWith<VelocityComponent>())
		{
			bool hasSphere = world.HasComponents<SphereCollider>(entity);
			if (!hasSphere)
			{
				Move(entity, deltaTime);
			}
			else
			{
				dynamicCollidables.push_back(entity);
			}
		}

		for (auto& e : dynamicCollidables)
		{
			Move(e, deltaTime);
		}

		auto cameraSize = Renderer::GetInstance().GetCameraSize();
		std::vector<CollisionPair> collisionCandidates;
		{
			auto colliders = world.GetEntitiesWith<SphereCollider>();
			m_quadTree = std::make_shared<QuadTreeNode>(colliders, Rect(-1000, -1000, 2000, 2000));
			for (size_t i = 0; i < colliders.size(); ++i)
			{
				Entity eA = colliders[i];
				auto others = m_quadTree->GetNeighbours(eA);
				for (auto& eB : others)
				{
					if (eB.GetID() > eA.GetID())
						collisionCandidates.push_back({ eA, eB });
				}
			}
		}

		// Broad-phase collision detection
		// For now N^2 brute force, but can be optimized
		/*std::vector<CollisionPair> collisionCandidates;
		{
			auto colliders = world.GetEntitiesWith<SphereCollider>();
			for (size_t i = 0; i < colliders.size(); ++i)
			{
				for (size_t j = i + 1; j < colliders.size(); ++j)
				{
					auto eA = colliders[i];
					auto eB = colliders[j];

					bool aIsDynamic = std::find(dynamicCollidables.begin(), dynamicCollidables.end(), eA) != dynamicCollidables.end();
					bool bIsDynamic = std::find(dynamicCollidables.begin(), dynamicCollidables.end(), eB) != dynamicCollidables.end();
					if (!aIsDynamic && !bIsDynamic)
						continue;

					collisionCandidates.push_back({ eA, eB });
				}
			}
		}*/

		std::vector<CollisionEvent> collisionEvents;
		std::vector<TriggerEvent> triggerEvents;

		for (auto& [eA, eB] : collisionCandidates)
		{
			const auto& colA = world.GetComponent<SphereCollider>(eA);
			const auto& colB = world.GetComponent<SphereCollider>(eB);

			if (CheckSphereSphereCollision(eA, eB))
			{
				bool aIsTrigger = colA->isTrigger;
				bool bIsTrigger = colB->isTrigger;

				if (aIsTrigger || bIsTrigger)
				{
					HandleTrigger(eA, eB, triggerEvents);
				}
				else
				{
					ResolveSphereSphereCollision(eA, eB);
					glm::vec2 dir = glm::normalize(world.GetComponent<Transform>(eA)->position - world.GetComponent<Transform>(eB)->position);
					CollisionEvent event(eA, eB, dir * colB->radius, dir);
					collisionEvents.push_back(event);
				}
			}
			else
			{
				RemoveInactiveTriggers(eA, eB);
			}
		}

		for (auto& ev : triggerEvents)
		{
			EventManager::GetInstance().AddEvents<TriggerEvent>(ev);
		}

		for (auto& ev : collisionEvents)
		{
			EventManager::GetInstance().AddEvents<CollisionEvent>(ev);
		}
	}

	bool PhysicsSystem::CheckSphereSphereCollision(Entity eA, Entity eB)
	{
		auto& world = Ecs::GetInstance();
		const auto& colliderA = world.GetComponent<SphereCollider>(eA);
		const auto& colliderB = world.GetComponent<SphereCollider>(eB);

		float distance = glm::distance(colliderA->transform.position, colliderB->transform.position);
		float radiusSum = colliderA->radius + colliderB->radius;
		return distance <= radiusSum;
	}

	void PhysicsSystem::ResolveSphereSphereCollision(Entity eA, Entity eB)
	{
		auto& world = Ecs::GetInstance();

		auto transformA = world.GetComponent<Transform>(eA);
		auto transformB = world.GetComponent<Transform>(eB);

		auto colliderA = world.GetComponent<SphereCollider>(eA);
		auto colliderB = world.GetComponent<SphereCollider>(eB);

		glm::vec2 posA = colliderA->transform.position;
		glm::vec2 posB = colliderB->transform.position;

		glm::vec2 delta = posA - posB;
		float dist = glm::length(delta);
		if (dist == 0.0f)
		{
			delta = glm::vec2(1.0f, 0.0f);
			dist = 0.0001f;
		}

		float overlap = (colliderA->radius + colliderB->radius) - dist;
		glm::vec2 correction = glm::normalize(delta) * overlap * 0.5f;

		transformA->position += correction;
		colliderA->transform.position += correction;
		transformB->position -= correction;
		colliderB->transform.position -= correction;
	}

	void PhysicsSystem::HandleTrigger(Entity eA, Entity eB, std::vector<TriggerEvent>& triggerEvents)
	{
		auto& world = Ecs::GetInstance();
		auto colA = world.GetComponent<SphereCollider>(eA);
		auto colB = world.GetComponent<SphereCollider>(eB);

		if (colA->isTrigger)
		{
			if (m_activeTriggers[eA].find(eB) == m_activeTriggers[eA].end())
			{
				m_activeTriggers[eA].insert(eB);
				triggerEvents.emplace_back(eA, eB);
			}
		}
		if (colB->isTrigger)
		{
			if (m_activeTriggers[eB].find(eA) == m_activeTriggers[eB].end())
			{
				m_activeTriggers[eB].insert(eA);
				triggerEvents.emplace_back(eB, eA);
			}
		}
	}

	void PhysicsSystem::RemoveInactiveTriggers(Entity eA, Entity eB)
	{
		if (m_activeTriggers[eA].find(eB) != m_activeTriggers[eA].end())
		{
			m_activeTriggers[eA].erase(eB);
		}
		if (m_activeTriggers[eB].find(eA) != m_activeTriggers[eB].end())
		{
			m_activeTriggers[eB].erase(eA);
		}
	}

	void PhysicsSystem::Move(Entity entity, float deltaTime)
	{
		World& world = Ecs::GetInstance();
		auto& objectPos = world.GetComponent<Transform>(entity)->position;
		auto vel = world.GetComponent<VelocityComponent>(entity);

		glm::vec2 displacement = vel->dir * vel->speed * deltaTime;
		objectPos += displacement;

		if (world.HasComponents<SphereCollider>(entity))
		{
			auto& colliderPos = world.GetComponent<SphereCollider>(entity)->transform.position;
			colliderPos += displacement;
		}
	}

}

