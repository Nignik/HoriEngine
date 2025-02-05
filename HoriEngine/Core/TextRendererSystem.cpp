#include "TextRendererSystem.h"
#include "Transform.h"
#include "TextComponent.h"
#include "Shader.h"
#include "Renderer.h"

#include <freetype/freetype.h>
#include <iostream>
#include <filesystem>
#include <ResourceManager.h>
#include <glm/glm.hpp>

namespace fs = std::filesystem;

namespace Hori
{

	TextRendererSystem::TextRendererSystem()
		: m_buffer(std::make_shared<OpenGLBuffer>())
	{
		m_shader = LoadShaderFromFile("shaders/text.vs", "shaders/text.fs");
		//glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenSize.x), 0.0f, static_cast<float>(screenSize.y));
		glm::mat4 projection = Renderer::GetInstance().GetProjectionMatrix();
		m_shader.Use();
		m_shader.SetMatrix4("projection", projection);

		InitializeFreeType();
		InitializeQuad();
	}

	void TextRendererSystem::Update(float dt)
	{
		auto& world = World::GetInstance();

		for (auto& entity : world.GetEntitiesWithComponents<TextComponent>())
		{
			RenderText(entity);
		}
	}

	void TextRendererSystem::InitializeFreeType()
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}

		fs::path font_path = "resources/fonts/Antonio-Bold.ttf";
		if (font_path.empty())
		{
			std::cerr << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		}

		FT_Face face;
		if (FT_New_Face(ft, font_path.string().c_str(), 0, &face))
		{
			std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}
		else
		{
			FT_Set_Pixel_Sizes(face, 0, 48);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (unsigned char c = 0; c < 128; c++)
			{
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cerr << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void TextRendererSystem::InitializeQuad()
	{
		glBindVertexArray(m_buffer->vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void TextRendererSystem::RenderText(Entity entity)
	{
		auto& world = World::GetInstance();

		auto text = world.GetComponent<TextComponent>(entity);
		auto transform = world.GetComponent<Transform>(entity);

		m_shader.Use();
		m_shader.SetVector3f("textColor", text->color);
		m_shader.SetInteger("text", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_buffer->vao);

		float x = transform->position.x;
		float y = transform->position.y;
		for (auto& c : text->text)
		{
			Character ch = Characters[c];

			float xpos = x + ch.bearing.x * transform->scale.x;
			float ypos = y - (ch.size.y - ch.bearing.y) * transform->scale.y;

			float w = ch.size.x * transform->scale.x;
			float h = ch.size.y * transform->scale.y;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};

			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			glBindBuffer(GL_ARRAY_BUFFER, m_buffer->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			x += (ch.advance >> 6) * transform->scale.x;
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

