#include "SpriteRenderer.h"
#include "Renderer.h"

#include <World.h>
#include <iostream>


namespace Hori
{

	SpriteRenderer::SpriteRenderer()
		: m_quad(std::make_shared<OpenGLBuffer>())
	{
		std::vector<float> vertices = {
			// pos                // tex
			-0.5f,  0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  1.0f, 0.0f
		};

		glGenVertexArrays(1, &m_quad->vao);
		glGenBuffers(1, &m_quad->vbo);

		glBindVertexArray(m_quad->vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_quad->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SpriteRenderer::Update(float deltaTime)
	{
		for (const auto& entity : Ecs::GetInstance().GetEntitiesWith<SpriteComponent>())
		{
			DrawSprite(entity);
		}
	}

	void SpriteRenderer::DrawSprite(const Entity& entity)
	{
		auto shader = Ecs::GetInstance().GetComponent<ShaderComponent>(entity);
		auto transform = Ecs::GetInstance().GetComponent<TransformComponent>(entity);
		auto sprite = Ecs::GetInstance().GetComponent<SpriteComponent>(entity);

		glm::mat4 projection = Hori::Renderer::GetInstance().GetProjectionMatrix();

		shader->Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(transform->position, 0.0f));
		model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0, 0.0, 1.0));
		
		model = glm::scale(model, glm::vec3(transform->scale, 1.0f));

		shader->SetMatrix4("model", model);
		shader->SetVector3f("spriteColor", glm::vec3(1.0f, 0.0f, 0.0f));
		shader->SetMatrix4("projection", projection);
		shader->SetInteger("sprite", 0);

		glActiveTexture(GL_TEXTURE0);
		sprite->Bind();

		glBindVertexArray(m_quad->vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

}



