#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

#include "stb_image.h"
#include "Components.h"

#include <filesystem>

namespace Hori
{
	template <typename T, typename... Ts>
	concept AnyOf = (std::same_as<T, Ts> || ...);

	template <typename T>
	concept ResourceTypes = AnyOf<T, ShaderComponent, SpriteComponent, YAML::Node>;

	template<typename T>
	struct ResourceHandle
	{
		uint32_t id{ 0 };

		bool isValid() const { return id != 0; }
		bool operator==(const ResourceHandle& other) const { return id == other.id; }
	};
}

namespace std {
    template<typename T>
    struct hash<Hori::ResourceHandle<T>> {
        size_t operator()(const Hori::ResourceHandle<T>& handle) const noexcept {
            return std::hash<uint32_t>{}(handle.id);
        }
    };
}

namespace Hori {
	template <typename T>
	class ResourceStorage
	{
	public:
		ResourceStorage() = default;
		[[nodiscard]] ResourceHandle<T> Add(const std::filesystem::path& path, std::shared_ptr<T> resource)
		{
			auto it = m_pathToHandle.find(path);
			if (it != m_pathToHandle.end())
			{
				std::cout << "Warning: Tried to add resource that already exists in the storage\n";
				return it->second;
			}

			ResourceHandle<T> handle{ m_nextId++ };
			m_resources.emplace(handle, resource);
			m_pathToHandle.emplace(path, handle);

			return handle;
		}

		std::shared_ptr<T> GetResource(const ResourceHandle<T> handle)
		{
			auto it = m_resources.find(handle);
			if (it == m_resources.end())
			{
				return nullptr;
			}

			return it->second;
		}

		ResourceHandle<T> GetHandle(const std::filesystem::path& path)
		{
			auto it = m_pathToHandle.find(path);
			if (it == m_pathToHandle.end())
			{
				std::cout << "Warning: Tried to get handle that doesn't exist" << path << '\n';
				return ResourceHandle<T>{0};
			}

			return it->second;
		}

		bool IsResourceLoaded(const std::filesystem::path& path)
		{
			return m_pathToHandle.contains(path);
		}

	protected:
		uint32_t m_nextId{ 1 };
		std::unordered_map<ResourceHandle<T>, std::shared_ptr<T>> m_resources{};
		std::unordered_map<std::filesystem::path, ResourceHandle<T>> m_pathToHandle{};

	};

	class ResourceManager
	{
	public:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		static ResourceManager& GetInstance()
		{
			static ResourceManager instance;
			return instance;
		}

		template<typename T>
			requires ResourceTypes<T>
		ResourceHandle<T> Load(const std::filesystem::path& path)
		{
			if constexpr (std::same_as<T, ShaderComponent>)						return LoadShaderFromFile(path);
			else if constexpr (std::same_as<T, SpriteComponent>)				return LoadTextureFromFile(path, true);
			else if constexpr (std::same_as<T, YAML::Node>)				return LoadYaml(path);
		}

		template<typename T>
			requires ResourceTypes<T>
		std::shared_ptr<T> Get(ResourceHandle<T> handle)
		{
			if constexpr (std::same_as<T, ShaderComponent>)						return m_shaderStorage.GetResource(handle);
			else if constexpr (std::same_as<T, SpriteComponent>)				return m_textureStorage.GetResource(handle);
			else if constexpr (std::same_as<T, YAML::Node>)				return m_yamlStorage.GetResource(handle);

			return nullptr;
		}

	private:
		ResourceManager() = default;
		~ResourceManager() = default;

		ResourceStorage<ShaderComponent> m_shaderStorage;
		ResourceStorage<SpriteComponent> m_textureStorage;
		ResourceStorage<YAML::Node> m_yamlStorage;

		ResourceHandle<ShaderComponent> LoadShaderFromFile(std::filesystem::path path)
		{
			if (m_shaderStorage.IsResourceLoaded(path))
			{
				return m_shaderStorage.GetHandle(path);
			}
		
			std::filesystem::path vShaderPath = path; 
			std::filesystem::path fShaderPath = path; 
			std::filesystem::path gShaderPath = path; 
			vShaderPath.replace_extension(".vs");
			fShaderPath.replace_extension(".fs");
			gShaderPath.replace_extension(".gs");

			std::string vertexCode;
			std::string fragmentCode;
			std::string geometryCode;

			// open files
			std::ifstream vertexShaderFile(vShaderPath);
			std::ifstream fragmentShaderFile(fShaderPath);
			std::stringstream vShaderStream, fShaderStream;

			// read file's buffer contents into streams
			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();

			// close file handlers
			vertexShaderFile.close();
			fragmentShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			// if geometry shader path is present, also load a geometry shader
			if (std::filesystem::exists(gShaderPath))
			{
				std::ifstream geometryShaderFile(gShaderPath);
				std::stringstream gShaderStream;
				gShaderStream << geometryShaderFile.rdbuf();
				geometryShaderFile.close();
				geometryCode = gShaderStream.str();
			}

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();
			const char* gShaderCode = geometryCode.c_str();

			// 2. now create shader object from source code
			auto shader = std::make_shared<ShaderComponent>();
			shader->Compile(vShaderCode, fShaderCode, std::filesystem::exists(gShaderPath) ? gShaderCode : nullptr);
			
			auto handle = m_shaderStorage.Add(path, shader);

			return handle;
		}

		ResourceHandle<YAML::Node> LoadYaml(const std::filesystem::path& path)
		{
			if (m_yamlStorage.IsResourceLoaded(path))
			{
				return m_yamlStorage.GetHandle(path);
			}

			auto resource = std::make_shared<YAML::Node>(YAML::LoadFile(path.string()));
			auto handle = m_yamlStorage.Add(path, resource);

			return handle;
		}

		ResourceHandle<SpriteComponent> LoadTextureFromFile(std::filesystem::path path, bool alpha)
		{
			if (m_textureStorage.IsResourceLoaded(path))
			{
				return m_textureStorage.GetHandle(path);
			}

			auto texture = std::make_shared<SpriteComponent>();
			if (alpha)
			{
				texture->InternalFormat = GL_RGBA;
				texture->imageFormat = GL_RGBA;
			}

			// load image
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);

			// now generate texture
			texture->Generate(width, height, data);

			// and finally free image data
			stbi_image_free(data);

			auto handle = m_textureStorage.Add(path, texture);

			return handle;
		}
	};
}
