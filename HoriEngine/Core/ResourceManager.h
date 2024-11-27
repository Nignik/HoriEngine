#pragma once

#include <memory>
#include <string>

#include "Shader.h"
#include "Texture.h"
#include "stb_image.h"

namespace Hori
{
	Shader LoadShaderFromFile(std::string vShaderFile, std::string fShaderFile, std::string gShaderFile = "");
	std::shared_ptr<Texture2D> LoadTextureFromFile(std::string file, bool alpha);
}