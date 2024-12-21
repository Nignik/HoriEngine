#include "HoriEngine.h"

#include "PhysicsSystem.h"
#include "SpriteRenderer.h"
#include "ActionSystem.h"

namespace Hori
{
	Engine::Engine()
		: m_prevTime(std::chrono::high_resolution_clock::now())
	{
	}

	Engine::~Engine()
	{
	}

	Engine& Engine::GetInstance()
	{
		static Engine instance;
		return instance;
	}

	void Engine::InitSystems()
	{
		glfwSetKeyCallback(Renderer::GetInstance().GetWindow(), key_callback);

		World::GetInstance().AddSystem<PhysicsSystem>(PhysicsSystem());
		World::GetInstance().AddSystem<SpriteRenderer>(SpriteRenderer());
		World::GetInstance().AddSystem<ActionSystem>(ActionSystem());

		World::GetInstance().AddSingletonComponent(InputComponent());
	}

	void Engine::Run()
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		
		while (!Renderer::GetInstance().ShouldClose())
		{
			currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaTime = currentTime - m_prevTime;
			m_prevTime = currentTime;

			Renderer::GetInstance().StartFrame();
			World::GetInstance().UpdateSystems(deltaTime.count());
			Renderer::GetInstance().EndFrame();
			EventManager::GetInstance().Clear();
		}
	}

	void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		auto& input = World::GetInstance().GetSingletonComponent<InputComponent>()->input;

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				input[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				input[key] = false;
			}
		}
	}

}
