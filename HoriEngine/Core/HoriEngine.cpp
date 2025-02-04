#include "HoriEngine.h"

#include "PhysicsSystem.h"
#include "SpriteRenderer.h"
#include "DebugRendererSystem.h"
#include "ActionSystem.h"
#include "DebugUISystem.h"
#include "DebugUIComponents.h"
#include "DebugRendererComponent.h"

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

		auto& world = World::GetInstance();
		world.AddSystem<SpriteRenderer>(SpriteRenderer());
		world.AddSystem<PhysicsSystem>(PhysicsSystem());
		world.AddSystem<ActionSystem>(ActionSystem());
	}

	void Engine::InitDebugSystems()
	{
		auto& world = World::GetInstance();

		world.AddSystem<DebugUISystem>(DebugUISystem(Renderer::GetInstance().GetWindow()));
		world.AddSystem<DebugRendererSystem>(DebugRendererSystem());

		world.AddSingletonComponent(DebugRendererComponent());

		m_debugUI = world.CreateEntity();
		ButtonComponent showColliders("Show collider wireframes", std::bind(&DebugRendererComponent::Switch, world.GetSingletonComponent<DebugRendererComponent>(), DebugDraw::COLLIDER_WIREFRAME));
		world.AddComponents(m_debugUI, showColliders);
	}

	void Engine::InitSingletonComponents()
	{
		auto& world = World::GetInstance();
		world.AddSingletonComponent(InputComponent());
		world.AddSingletonComponent(QuadTreeComponent());
	}

	void Engine::Run()
	{
		auto currentTime = std::chrono::high_resolution_clock::now();

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(Renderer::GetInstance().GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
		
		while (!Renderer::GetInstance().ShouldClose())
		{
			currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaTime = currentTime - m_prevTime;
			m_prevTime = currentTime;

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Renderer::GetInstance().StartFrame();

			World::GetInstance().UpdateSystems(deltaTime.count());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			Renderer::GetInstance().EndFrame();
			EventManager::GetInstance().Clear();
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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
