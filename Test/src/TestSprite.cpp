#include <World.h>
#include <Core/Sprite.h>
#include <Core/HoriEngine.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>
#include <Core/Controller.h>

#include <gtest/gtest.h>

TEST(TestSprite, RenderingSprites)
{
	auto& engine = Hori::Engine::GetInstance();
	engine.InitSystems();

	auto& renderer = Hori::Renderer::GetInstance();

	auto& world = Hori::World::GetInstance();
	auto newEntity = world.CreateEntity();

	Transform transform = {
		.position = {200.0f, 200.0f},
		.rotation = 0.0f,
		.scale = {200.0f, 200.0f}
	};
	Hori::Shader shader = Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs");
	Hori::Texture2D texture = Hori::LoadTextureFromFile("resources/sprites/awesomeface.png", true);
	
	world.AddComponent(newEntity, Hori::Sprite());
	world.AddComponent(newEntity, Hori::Controller());
	world.AddComponent(newEntity, shader);
	world.AddComponent(newEntity, transform);
	world.AddComponent(newEntity, texture);
	
	engine.Run();
}
