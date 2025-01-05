#pragma once

#include <string>
#include <functional>

namespace Hori
{
	struct ButtonComponent
	{
		std::string label{};
		std::function<void()> onClick{};
	};
}