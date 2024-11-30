#include "ActionSystem.h"

#include <GLFW/glfw3.h>
#include <InputComponent.h>
#include <World.h>
#include <controller.h>
#include <Transform.h>

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
		auto& transform = World::GetInstance().GetComponent<Transform>(entity);

		if (input[GLFW_KEY_W])
		{
			transform.position.y -= 5.0f;
		}
		if (input[GLFW_KEY_S])
		{
			transform.position.y += 5.0f;
		}
		if (input[GLFW_KEY_D])
		{
			transform.position.x += 5.0f;
		}
		if (input[GLFW_KEY_A])
		{
			transform.position.x -= 5.0f;
		}
	}

}