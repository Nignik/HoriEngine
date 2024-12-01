#pragma once

#include "Transform.h"

namespace Hori
{
	struct BoxCollider
	{
		Transform transform{};
		glm::vec2 size{};
	};

	struct SphereCollider
	{
		Transform transform{};
		float radius{};
	};
}
