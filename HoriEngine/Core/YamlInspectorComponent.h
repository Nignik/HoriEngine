#pragma once

#include <imgui.h>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <yaml-cpp//yaml.h>
#include <fstream>

#include "ResourceManager.h"

namespace fs = std::filesystem;

namespace Hori
{
	class YamlInspectorNode : public std::enable_shared_from_this<YamlInspectorNode>
	{
	public:
		YamlInspectorNode(YAML::Node node, const std::filesystem::path& filePath)
			: node(node),
			filePath(filePath)
		{
		}

		void Init(std::shared_ptr<YamlInspectorNode> rootPtr = nullptr)
		{
			if (!rootPtr)
			{
				root = shared_from_this();
			}
			else
			{
				root = rootPtr;
			}

			if (node.IsMap())
			{
				for (const auto& child : node)
				{
					std::string childLabel = child.first.as<std::string>();

					auto childNode = std::make_shared<YamlInspectorNode>(child.second, filePath);
					childNode->Init(root);
					childNode->label = childLabel;

					if (child.second.IsScalar())
					{
						childNode->inputBuffer = child.second.as<std::string>();
					}

					children.push_back(childNode);
				}
			}
			else if (node.IsSequence())
			{
				for (size_t i = 0; i < node.size(); ++i)
				{
					auto childNode = std::make_shared<YamlInspectorNode>(node[i], filePath);
					childNode->Init(root);
					childNode->label = "[" + std::to_string(i) + "]";
					children.push_back(childNode);
				}
			}
			else if (node.IsScalar())
			{
				label = node.as<std::string>();
			}
		}

		static int InputTextCallback(ImGuiInputTextCallbackData* data)
		{
			YamlInspectorNode* node = static_cast<YamlInspectorNode*>(data->UserData);
			if (node)
			{
				node->SetBuffer(data);
			}
			return 0;
		}

		void SetBuffer(ImGuiInputTextCallbackData* data)
		{
			if (data->EventFlag & ImGuiInputTextFlags_CallbackEdit)
			{
				node = data->Buf;
				YAML::Emitter out;
				out << root->node;
				std::ofstream fout(filePath);
				fout << out.c_str();
				fout.close();
			}
		}

		YAML::Node node{};
		std::string inputBuffer{};
		std::string label{};
		fs::path filePath{};
		std::shared_ptr<YamlInspectorNode> root = nullptr;
		std::vector<std::shared_ptr<YamlInspectorNode>> children{};
	};

	

	struct YamlInspectorComponent
	{
		void Open(const std::filesystem::path& path)
		{
			auto& resourceMng = ResourceManager::GetInstance();
			auto handle = resourceMng.Load<YAML::Node>(path);
			auto node = *resourceMng.Get(handle);

			root = std::make_shared<YamlInspectorNode>(node, path);
			root->Init();
			root->label = path.string();
		}
		
		std::shared_ptr<YamlInspectorNode> root{};
	};
}