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
		auto& input = World::GetInstance().GetSingletonComponent<InputComponent>().input;

		for (const auto& entity : World::GetInstance().GetEntitiesWithComponents<Controller>())
		{
			Move(entity, input);
		}
	}

	void ActionSystem::Move(const Entity& entity, std::vector<int>& input)
	{
		auto& velocity = World::GetInstance().GetComponent<VelocityComponent>(entity).velocity;
		velocity = { 0.0f, 0.0f };

		if (input[GLFW_KEY_W])
			velocity.y -= 25.0f;
		
		if (input[GLFW_KEY_S])
			velocity.y += 25.0f;
		
		if (input[GLFW_KEY_D])
			velocity.x += 25.0f;

		if (input[GLFW_KEY_A])
			velocity.x -= 25.0f;
	}

}