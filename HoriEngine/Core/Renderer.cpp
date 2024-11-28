#include "Renderer.h"

#include <cassert>
#include <vector>
#include <iostream>

namespace Hori
{
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	GLFWwindow* Renderer::m_window = nullptr;

	Renderer::Renderer()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, false);

		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;
			// Handle the error appropriately, e.g.,
			throw std::runtime_error("Failed to initialize GLFW");
		}

		m_window = glfwCreateWindow(800, 600, "Hori", nullptr, nullptr);

		// After glfwCreateWindow
		if (m_window == nullptr)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(m_window);
		glfwSetKeyCallback(m_window, key_callback);
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

		// For gladLoadGLLoader
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			glfwDestroyWindow(m_window);
			glfwTerminate();
			throw std::runtime_error("Failed to initialize GLAD");
		}

		glViewport(0, 0, 800, 600);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::EndFrame()
	{
		glfwSwapBuffers(m_window);
	}
}



