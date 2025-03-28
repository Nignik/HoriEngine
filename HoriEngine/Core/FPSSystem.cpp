#include "FPSSystem.h"
#include "Components.h"

#include <iostream>

namespace Hori
{

	FPSSystem::FPSSystem()
	{
		auto& world = Ecs::GetInstance();

		m_fpsEntity = world.CreateEntity();
		world.AddComponents(m_fpsEntity, Text("0", { 0.f, 1.f, 0.f }), Transform({ 60.f, 40.f }, 0.f, { 0.05f, 0.05f }), FramesCounter());
	}

	void FPSSystem::Update(float dt)
	{
		auto& world = Ecs::GetInstance();

		auto fps = world.GetComponent<FramesCounter>(m_fpsEntity);
		assert(fps != nullptr && "Error: using fps system without initializing singleton fps component");

		fps->timeCount += dt;
		fps->framesCount++;
		if (fps->timeCount >= 1.f)
		{
			fps->fps = fps->framesCount;
			world.GetComponent<Text>(m_fpsEntity)->text = std::to_string(fps->fps);
			fps->framesCount = 0;
			fps->timeCount = 0.f;
		}
	}
}


