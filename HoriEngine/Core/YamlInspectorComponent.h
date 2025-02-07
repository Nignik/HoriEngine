#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <yaml-cpp//yaml.h>

namespace fs = std::filesystem;

namespace Hori
{

	struct YamlInspectorComponent
	{
		std::string label{};
		YAML::Node root{};

		void OpenFile(fs::path filePath)
		{
			if (fs::exists(filePath))
			{
				label = filePath.string();
				root = YAML::LoadFile(filePath.string().c_str());
			}
			else
			{
				std::cout << "ERROR: File path passed to yaml inspector doesnt exist" << std::endl;
			}
		}
	};
}