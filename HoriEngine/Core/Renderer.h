#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hori
{
	class Renderer
	{
	public:
		Renderer();

		bool ShouldClose();

		void RenderFrame();

	private:
		GLFWwindow* m_window = nullptr;
	};
}