#pragma once

#include <unordered_map>

namespace Hori
{
	enum class DebugDraw
	{
		COLLIDER_WIREFRAME
	};

	struct DebugRendererComponent
	{
		void Switch(DebugDraw option)
		{
			enabled[option] = !enabled[option];
		}

		std::unordered_map<DebugDraw, bool> enabled{};
	};
}
