#include "DebugRendererSystem.h"

#include "Transform.h"
#include "DebugRendererComponent.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Renderer.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_glfw.h>

namespace Hori
{
	DebugRendererSystem::DebugRendererSystem()
		: m_shader(LoadShaderFromFile("shaders/wireframe.vs", "shaders/wireframe.fs"))
	{

	}

 	void DebugRendererSystem::Update(float dt)
 	{
 		auto& world = World::GetInstance();
		
		auto& enabled = world.GetSingletonComponent<DebugRendererComponent>()->enabled;
		if (enabled[DebugDraw::COLLIDER_WIREFRAME])
			for (auto& entity : world.GetEntitiesWithComponents<WireframeComponent>())
				RenderWireframe(entity);
 	}

	void DebugRendererSystem::RenderWireframe(Entity entity)
	{
		auto transform = World::GetInstance().GetComponent<Transform>(entity);
		auto wireframe = World::GetInstance().GetComponent<WireframeComponent>(entity);

		glm::mat4 projection = Hori::Renderer::GetInstance().GetProjectionMatrix();
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(transform->position, 0.0f));
		model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, glm::vec3(transform->scale, 1.0f));

		m_shader.Use();
		m_shader.SetMatrix4("model", model);
		m_shader.SetMatrix4("projection", projection);
		m_shader.SetVector3f("color", glm::vec3(0.3f, 0.4f, 0.0f));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.0f);
		glBindVertexArray(wireframe->buff->vao);
		glDrawArrays(GL_LINE_LOOP, 0, wireframe->vertexCount);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(0);
	}
}

