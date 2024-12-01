#pragma once

#include <glm/glm.hpp>

namespace Hori
{
	struct Transform
	{
		glm::vec2 position{};
		float rotation{};
		glm::vec2 scale{};
	};
}
