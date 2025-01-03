#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"
#include "Texture.h"
#include "stb_image.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Hori
{
	inline Shader LoadShaderFromFile(fs::path vShaderFile, fs::path fShaderFile, fs::path gShaderFile = "")
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		try
		{
			// open files
			std::ifstream vertexShaderFile(vShaderFile);
			std::ifstream fragmentShaderFile(fShaderFile);
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
			if (gShaderFile != "")
			{
				std::ifstream geometryShaderFile(gShaderFile);
				std::stringstream gShaderStream;
				gShaderStream << geometryShaderFile.rdbuf();
				geometryShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (fs::filesystem_error& e)
		{
			std::cout << "EXCEPTION: " << e.what() << std::endl;
			std::cout << "     path: " << e.path1() << std::endl;
		}
		catch (...) {
			std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		const char* gShaderCode = geometryCode.c_str();
		// 2. now create shader object from source code
		Shader shader;
		shader.Compile(vShaderCode, fShaderCode, gShaderFile != "" ? gShaderCode : nullptr);
		return shader;
	}

	inline Texture2D LoadTextureFromFile(fs::path file, bool alpha)
	{
		Texture2D texture;
		if (alpha)
		{
			texture.InternalFormat = GL_RGBA;
			texture.imageFormat = GL_RGBA;
		}
		// load image
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(file.string().c_str(), &width, &height, &nrChannels, 0);
		// now generate texture
		texture.Generate(width, height, data);
		// and finally free image data
		stbi_image_free(data);
		return texture;
	}
}