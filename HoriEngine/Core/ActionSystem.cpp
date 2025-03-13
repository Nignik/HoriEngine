#include "ActionSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <World.h>
#include <iostream>

#include "Components.h"

namespace Hori
{

	void ActionSystem::Update(float deltaTime)
	{
		auto& input = Ecs::GetInstance().GetSingletonComponent<InputComponent>()->input;
		
		for (const auto& entity : Ecs::GetInstance().GetEntitiesWith<ControllerComponent>())
		{
			Move(entity, input);
		}
	}

	void ActionSystem::Move(const Entity& entity, std::vector<int>& input)
	{
		auto velocity = Ecs::GetInstance().GetComponent<VelocityComponent>(entity);
		*velocity = { {0.0f, 0.0f}, velocity->speed };

		if (input[GLFW_KEY_W])
			velocity->dir.y += 1.0f;
		
		if (input[GLFW_KEY_S])
			velocity->dir.y -= 1.0f;
		
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