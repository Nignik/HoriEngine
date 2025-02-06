#pragma once

#include <World.h>

namespace Hori
{
	class FPSSystem : public System
	{
	public:
		FPSSystem();
		void Update(float dt) override;

	private:
		Entity m_fpsEntity{};
	};
}
