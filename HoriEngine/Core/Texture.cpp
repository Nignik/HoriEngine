#include <iostream>

#include "texture.h"

namespace Hori
{
	Texture2D::Texture2D()
		: Width(0), Height(0), InternalFormat(GL_RGB), imageFormat(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
	{
		glGenTextures(1, &this->Id);
	}

	void Texture2D::Generate(uint32_t width, uint32_t height, unsigned char* data)
	{
		this->Width = width;
		this->Height = height;
		// create Texture
		glBindTexture(GL_TEXTURE_2D, this->Id);
		glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
		// set Texture wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, this->Id);
	}
}

