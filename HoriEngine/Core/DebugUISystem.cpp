#include "DebugUISystem.h"
#include "DebugUIComponents.h"

#include <imgui.h>
#include <EventManager.h>

namespace Hori
{

	DebugUISystem::DebugUISystem(GLFWwindow* window)
	{
		
	}

	DebugUISystem::~DebugUISystem()
	{
		
	}

	void DebugUISystem::Update(float dt)
	{
		auto& world = World::GetInstance();
		for (auto entity : world.GetEntitiesWithComponents<ButtonComponent>())
		{
			auto button = world.GetComponent<ButtonComponent>(entity);
			if (ImGui::Button((button->label).c_str()))
			{
				button->onClick();
			}
		}
	}
}