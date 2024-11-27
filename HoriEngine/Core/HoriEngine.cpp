#include "HoriEngine.h"

#include "PhysicsSystem.h"
#include "SpriteRenderer.h"

namespace Hori
{
	extern World* g_world;

	Engine::Engine()
		: m_prevTime(std::chrono::high_resolution_clock::now())
	{
	}

	Engine::~Engine()
	{
	}

	void Engine::InitSystems()
	{
		World::GetInstance().AddSystem<PhysicsSystem>(PhysicsSystem());
		World::GetInstance().AddSystem<SpriteRenderer>(SpriteRenderer());
	}

	void Engine::Run()
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - m_prevTime;

		while (!Renderer::GetInstance().ShouldClose())
		{
			Renderer::GetInstance().StartFrame();
			World::GetInstance().UpdateSystems(deltaTime.count());
			Renderer::GetInstance().EndFrame();
		}

		
	}

}
