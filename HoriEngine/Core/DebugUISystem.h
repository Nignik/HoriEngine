#pragma once

#include <World.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_glfw.h>

#include "YamlInspectorComponent.h"

namespace Hori
{
	class DebugUISystem : public System
	{
	public:
		DebugUISystem(GLFWwindow* window);
		~DebugUISystem();

		void Update(float dt) override;

	private:
		void RenderButton(Entity entity);
		void RenderFileBrowser(Entity entity);
		void RenderYamlInspector(Entity entity);
		void RenderYamlInspectorNode(YamlInspectorNode& node, int& idx);
	};
}
