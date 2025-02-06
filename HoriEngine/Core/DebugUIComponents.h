#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace Hori
{
	struct ButtonComponent
	{
		std::string label{};
		std::function<void()> onClick{};
	};

	struct FileBrowserComponent
	{
		FileBrowserComponent(std::string&& label, std::string&& defaultPath)
			: label(label),
			defaultPath(defaultPath),
			path(defaultPath)
		{
			if (!fs::exists(defaultPath))
				std::cout << "ERROR: The default path provided for file browser doesnt exist" << std::endl;

		}
		std::string label{};
		fs::path defaultPath{};
		fs::path path{};
		fs::path selectedFilePath{};
	};
}