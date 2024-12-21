#include "ActionSystem.h"

#include <GLFW/glfw3.h>
#include <InputComponent.h>
#include <World.h>
#include <controller.h>
#include <Transform.h>
#include "VelocityComponent.h"

#include <iostream>

namespace Hori
{

	void ActionSystem::Update(float deltaTime)
	{
		auto& input = World::GetInstance().GetSingletonComponent<InputComponent>()->input;

		for (const auto& entity : World::GetInstance().GetEntitiesWithComponents<Controller>())
		{
			Move(entity, input);
		}
	}

	void ActionSystem::Move(const Entity& entity, std::vector<int>& input)
	{
		auto velocity = World::GetInstance().GetComponent<VelocityComponent>(entity);
		*velocity = { {0.0f, 0.0f}, velocity->speed };

		if (input[GLFW_KEY_W])
			velocity->dir.y -= 1.0f;
		
		if (input[GLFW_KEY_S])
			velocity->dir.y += 1.0f;
		
		if (input[GLFW_KEY_D])
			velocity->dir.x += 1.0f;

		if (input[GLFW_KEY_A])
			velocity->dir.x -= 1.0f;

		if (glm::length(velocity->dir) > 0.0f)
			velocity->dir = glm::normalize(velocity->dir);
		else
			velocity->dir = { 0.0f, 0.0f };
	}

}