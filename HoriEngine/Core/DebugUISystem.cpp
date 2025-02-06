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
			RenderButton(entity);
		}

		for (auto entity : world.GetEntitiesWithComponents<FileBrowserComponent>())
		{
			RenderFileBrowser(entity);
		}
	}

	void DebugUISystem::RenderButton(Entity entity)
	{
		auto& world = World::GetInstance();

		auto button = world.GetComponent<ButtonComponent>(entity);
		if (ImGui::Button((button->label).c_str()))
		{
			button->onClick();
		}
	}

	void DebugUISystem::RenderFileBrowser(Entity entity)
	{
		auto& world = World::GetInstance();

		auto fileBrowser = world.GetComponent<FileBrowserComponent>(entity);
		if (ImGui::Begin(fileBrowser->label.c_str()))
		{
			if (ImGui::Button("<-"))
			{
				fileBrowser->path = fileBrowser->path.parent_path();
			}
			ImGui::SameLine();
			ImGui::Text("%s", fileBrowser->path.c_str());

			for (const auto& entry : fs::directory_iterator(fileBrowser->path))
			{
				const auto& path = entry.path();
				auto filename = path.filename().string();

				if (entry.is_directory())
				{
					if (ImGui::Button(filename.c_str()))
					{
						fileBrowser->path = path;
					}
				}
				else if (entry.is_regular_file())
				{
					ImGui::Selectable(filename.c_str(), fileBrowser->selectedFilePath == path);

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
					{
						fileBrowser->selectedFilePath = path;
					}
				}
			}
		}
		ImGui::End();
	}

}