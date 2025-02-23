#pragma once

#include <World.h>

namespace Hori
{
	class Ecs
	{
	public:
		Ecs(const Ecs&) = delete;
		Ecs& operator=(const Ecs&) = delete;

		static World& GetInstance()
		{
			static Ecs instance;
			return instance.m_world;
		}

	private:
		Ecs() = default;
		~Ecs() = default;

		World m_world{};
	};
}
