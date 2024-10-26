#include "Renderer.h"

#include <iostream>

namespace Hori
{
	Renderer::Renderer()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(700, 700, "Hori Invaders", nullptr, nullptr);
		if (m_window == nullptr)
		{
			std::cerr << "Failed to initialize window" << std::endl;
			return;
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			glfwTerminate();
			return;
		}
	}

	bool Renderer::ShouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	void Renderer::RenderFrame()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

}



