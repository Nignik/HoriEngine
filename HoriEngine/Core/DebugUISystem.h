#pragma once

#include <World.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_glfw.h>

namespace Hori
{
	class DebugUISystem : public System
	{
	public:
		DebugUISystem(GLFWwindow* window);
		~DebugUISystem();

		void Update(float dt) override;

	private:

	};
}
