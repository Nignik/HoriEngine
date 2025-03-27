#include "DebugRendererSystem.h"

#include "Components.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_glfw.h>

namespace Hori
{
	DebugRendererSystem::DebugRendererSystem()
	{
		auto& resourceMng = ResourceManager::GetInstance();
		std::filesystem::path shaderPath{ "shaders/wireframe" };
		m_shader = *resourceMng.Get<Shader>(shaderPath);
	}

 	void DebugRendererSystem::Update(float dt)
 	{
 		auto& world = Ecs::GetInstance();
		
		auto& enabled = world.GetSingletonComponent<DebugRenderer>()->enabled;
		if (enabled[DebugDraw::COLLIDER_WIREFRAME])
		{
			for (auto& entity : world.GetEntitiesWith<Wireframe>())
			{
				RenderWireframe(entity);
			}
		}
 	}

	void DebugRendererSystem::RenderWireframe(Entity entity)
	{
		auto transform = Ecs::GetInstance().GetComponent<Transform>(entity);
		auto wireframe = Ecs::GetInstance().GetComponent<Wireframe>(entity);

		glm::mat4 projection = Hori::Renderer::GetInstance().GetProjectionMatrix();
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(transform->position, 0.0f));
		model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, glm::vec3(transform->scale, 1.0f));

		m_shader.Use();
		m_shader.SetUniform("model", model);
		m_shader.SetUniform("projection", projection);
		m_shader.SetUniform("color", glm::vec3(0.3f, 0.4f, 0.0f));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.0f);
		glBindVertexArray(wireframe->buff->vao);
		glDrawArrays(GL_LINE_LOOP, 0, wireframe->vertexCount);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(0);
	}
}

