#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

namespace Hori
{
	Shader LoadShaderFromFile(std::string vShaderFile, std::string fShaderFile, std::string gShaderFile)
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
		catch (std::exception e)
		{
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

	std::shared_ptr<Hori::Texture2D> LoadTextureFromFile(std::string file, bool alpha)
	{
		// create texture object
		std::shared_ptr<Texture2D> texture;
		if (alpha)
		{
			texture->InternalFormat = GL_RGBA;
			texture->imageFormat = GL_RGBA;
		}
		// load image
		int width, height, nrChannels;
		unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
		// now generate texture
		texture->Generate(width, height, data);
		// and finally free image data
		stbi_image_free(data);
		return texture;
	}

}