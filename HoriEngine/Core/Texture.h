#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Hori
{
	class Texture2D
	{
	public:
		uint32_t Id;
		uint32_t Width, Height;
		uint32_t InternalFormat;
		uint32_t imageFormat;

		uint32_t WrapS;
		uint32_t WrapT;
		uint32_t FilterMin;
		uint32_t FilterMax;

		Texture2D();

		void Generate(uint32_t width, uint32_t height, unsigned char* data);

		void Bind() const;

	private:

	};
}