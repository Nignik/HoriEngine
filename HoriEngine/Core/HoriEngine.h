#pragma once

#include <World.h>
#include <chrono>

#include "Renderer.h"

namespace Hori
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		void InitSystems();

		void Run();

	private:
		std::chrono::steady_clock::time_point m_prevTime;
	};
}
