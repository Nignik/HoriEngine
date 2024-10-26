#pragma once

#include <System.h>

namespace Hori
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();

		void Update(float deltaTime) override;

	private:

	};
}