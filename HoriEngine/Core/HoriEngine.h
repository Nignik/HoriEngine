#pragma once

#include <World.h>
#include <chrono>

#include "Renderer.h"
#include "Shader.h"
#include "InputComponent.h"

namespace Hori
{
	class Engine
	{
	public:
		Engine(const Renderer&) = delete;
		Engine& operator=(const Renderer&) = delete;

		static Engine& GetInstance();

		void InitSystems();
		void Run();

	private:
		Engine();
		~Engine();

		std::chrono::steady_clock::time_point m_prevTime;

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	};
}
