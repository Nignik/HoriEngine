#include "FPSSystem.h"
#include "FPSComponent.h"

#include <iostream>

namespace Hori
{
	void FPSSystem::Update(float dt)
	{
		auto& world = World::GetInstance();

		auto fps = world.GetSingletonComponent<FPSComponent>();
		assert(fps != nullptr && "Error: using fps system without initializing singleton fps component");

		fps->timeCount += dt;
		fps->framesCount++;
		if (fps->timeCount >= 1.f)
		{
			fps->fps = fps->framesCount;
			std::cout << fps->framesCount << std::endl;
			fps->framesCount = 0;
			fps->timeCount = 0.f;
		}
	}
}


