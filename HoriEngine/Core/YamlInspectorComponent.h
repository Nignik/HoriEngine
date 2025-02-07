#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <yaml-cpp//yaml.h>

namespace fs = std::filesystem;

namespace Hori
{

	struct YamlInspectorNode
	{
		YAML::Node node{};
		std::string inputBuffer{};
		std::string label{};
		std::vector<YamlInspectorNode> children{};

		YamlInspectorNode(YAML::Node node)
			: node(node)
		{
			if (node.IsMap())
			{
				for (const auto& child : node)
				{
					std::string childLabel = child.first.as<std::string>();
					children.emplace_back(child.second);
					children.back().label = childLabel;
				}
			}
			else if (node.IsSequence())
			{
				for (std::size_t i = 0; i < node.size(); ++i)
				{
					children.emplace_back(node[i]);
					children.back().label = "[" + std::to_string(i) + "]";
				}
			}
			else if (node.IsScalar())
			{
				label = node.as<std::string>();
			}
		}
	};

	struct YamlInspectorComponent
	{
		std::string label{};
		std::optional<YamlInspectorNode> root;

		void OpenFile(fs::path filePath)
		{
			if (fs::exists(filePath))
			{
				label = filePath.string();
				auto node = YAML::LoadFile(filePath.string().c_str());
				root = YamlInspectorNode(node);
			}
			else
			{
				std::cout << "ERROR: File path passed to yaml inspector doesnt exist" << std::endl;
			}
		}
	};
}