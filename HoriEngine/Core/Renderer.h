#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hori
{
	class Renderer
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		static Renderer& GetInstance();

		bool ShouldClose();

		void StartFrame();
		void EndFrame();

	private:
		Renderer();
		~Renderer();

		static GLFWwindow* m_window;
	};
}