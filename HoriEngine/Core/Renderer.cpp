#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <vector>
#include <iostream>

namespace Hori
{
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	GLFWwindow* Renderer::m_window = nullptr;

	Renderer::Renderer()
		: m_windowSize(1280, 720)
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

		m_window = glfwCreateWindow((int)m_windowSize.x, (int)m_windowSize.y, "Hori", nullptr, nullptr);

		// After glfwCreateWindow
		if (m_window == nullptr)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

		// For gladLoadGLLoader
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			glfwDestroyWindow(m_window);
			glfwTerminate();
			throw std::runtime_error("Failed to initialize GLAD");
		}

		glViewport(0, 0, (int)m_windowSize.x, (int)m_windowSize.y);
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

	GLFWwindow* Renderer::GetWindow()
	{
		return m_window;
	}

	glm::vec2 Renderer::GetWindowSize()
	{
		return m_windowSize;
	}

	glm::vec2 Renderer::GetCameraSize()
	{
		return m_cameraSize;
	}

	glm::mat4 Renderer::GetProjectionMatrix()
	{
		return glm::ortho(-m_cameraSize.x/2, m_cameraSize.x/2, -m_cameraSize.y/2, m_cameraSize.y/2, -1.0f, 1.0f);
	}

}



