#include "SpriteRenderer.h"

#include <World.h>
#include <iostream>


namespace Hori
{
	void SpriteRenderer::Update(float deltaTime)
	{
		auto entities = World::GetInstance().GetEntitiesWithComponents<Sprite>();
		for (auto& entity : entities)
		{
			DrawSprite(entity);
		}
	}

	void SpriteRenderer::DrawSprite(Entity& entity)
	{
		Shader& shader = World::GetInstance().GetComponent<Shader>(entity);
		Transform& transform = World::GetInstance().GetComponent<Transform>(entity);
		Sprite& sprite = World::GetInstance().GetComponent<Sprite>(entity);

		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

		shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(transform.position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5 * transform.scale.x, 0.5 * transform.scale.y, 0.0));
		model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0.0, 0.0, 1.0));
		model = glm::translate(model, glm::vec3(-0.5 * transform.scale.x, -0.5 * transform.scale.y, 0.0));
		model = glm::scale(model, glm::vec3(transform.scale, 1.0f));

		shader.SetMatrix4("model", model);
		shader.SetVector3f("color", glm::vec3(1.0f, 0.0f, 0.0f));
		shader.SetMatrix4("projection", projection);

		/*glActiveTexture(GL_TEXTURE0);
		m_texture->Bind();*/
		glBindVertexArray(sprite.m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

}



