#pragma once

#include <World.h>
#include <chrono>

#include "Renderer.h"
#include "Components.h"
#include "QuadTreeComponent.h"
#include "Ecs.h"

namespace Hori
{
	class Engine
	{
	public:
		Engine(const Renderer&) = delete;
		Engine& operator=(const Renderer&) = delete;

		static Engine& GetInstance();

		void InitSystems();
		void InitDebugSystems();
		void InitSingletonComponents();
		void Run();

	private:
		Engine();
		~Engine();

		std::chrono::steady_clock::time_point m_prevTime;
		Entity m_debugUI{};

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	};
}
