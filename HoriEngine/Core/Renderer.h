#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

		GLFWwindow* GetWindow();
		glm::vec2 GetScreenSize();

	private:
		Renderer();
		~Renderer();

		glm::vec2 m_screenSize;

		static GLFWwindow* m_window;
	};
}