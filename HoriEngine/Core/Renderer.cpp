#include "Renderer.h"

#include <cassert>
#include <iostream>

namespace Hori
{
	GLFWwindow* Renderer::m_window = nullptr;

	Renderer::Renderer()
	{
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;
			assert(false);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(700, 700, "Hori Invaders", nullptr, nullptr);
		assert(m_window != nullptr);

		glfwMakeContextCurrent(m_window);

		assert(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));
	}

	Renderer::~Renderer()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}
		glfwTerminate();
	}

	Renderer& Renderer::GetInstance()
	{
		static Renderer instance;
		return instance;
	}

	bool Renderer::ShouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	void Renderer::StartFrame()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::EndFrame()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}



