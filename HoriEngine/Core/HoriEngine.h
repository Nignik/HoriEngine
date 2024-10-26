#pragma once

#include <World.h>

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
		std::unique_ptr<Renderer> m_renderer{};

	};
}
