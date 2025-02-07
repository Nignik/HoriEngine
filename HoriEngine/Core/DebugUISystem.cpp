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

		for (auto entity : world.GetEntitiesWithComponents<YamlInspectorComponent>())
		{
			RenderYamlInspector(entity);
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

	void DebugUISystem::RenderYamlInspector(Entity entity)
	{
		auto& world = World::GetInstance();
		auto yamlInspector = world.GetComponent<YamlInspectorComponent>(entity);

		if (ImGui::Begin("yaml inspector"))
		{
			int idx = 0;
			RenderYamlInspectorNode(yamlInspector->root, yamlInspector->label, idx);
		}
		ImGui::End();
	}

	void DebugUISystem::RenderYamlInspectorNode(const YAML::Node& node, const std::string& label, int& idx)
	{
		ImGui::PushID(idx++);

		if (node.IsScalar())
		{
			ImGui::Text("%s: %s", label.c_str(), node.as<std::string>().c_str());
			ImGui::PopID();
			return;
		}

		if (ImGui::TreeNode(label.c_str()))
		{
			if (node.IsMap())
			{
				for (auto const& kv : node)
				{
					auto childLabel = kv.first.as<std::string>();
					RenderYamlInspectorNode(kv.second, childLabel, idx);
				}
			}
			else if (node.IsSequence())
			{
				for (std::size_t i = 0; i < node.size(); ++i)
				{
					auto itemLabel = "[" + std::to_string(i) + "]";
					RenderYamlInspectorNode(node[i], itemLabel, idx);
				}
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}