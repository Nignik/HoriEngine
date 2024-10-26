#include "HoriEngine.h"

#include "PhysicsSystem.h"

namespace Hori
{
	extern World* g_world;

	Engine::Engine()
	{
	}

	Engine::~Engine()
	{
	}

	void Engine::InitSystems()
	{
		World::GetInstance().AddSystem(std::make_shared<PhysicsSystem>());
	}

	void Engine::Run()
	{
		while (!m_renderer->ShouldClose())
		{
			m_renderer->RenderFrame();
		}
	}

}
