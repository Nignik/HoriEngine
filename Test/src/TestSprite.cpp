#include <World.h>
#include <Core/Sprite.h>
#include <Core/HoriEngine.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>

#include <gtest/gtest.h>

TEST(TestSprite, RenderingSprites)
{
	Hori::Engine engine;
	engine.InitSystems();

	auto& renderer = Hori::Renderer::GetInstance();

	auto& world = Hori::World::GetInstance();
	auto newEntity = world.CreateEntity();

	Transform transform = {
		.position = {200.0f, 200.0f},
		.rotation = 0.0f,
		.scale = {300.0f, 400.0f}
	};
	Hori::Sprite sprite;
	Hori::Shader shader = Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs");

	//std::shared_ptr<Hori::Texture2D> texture = Hori::LoadTextureFromFile("xd", false);
	
	world.AddComponent(newEntity, sprite);
	world.AddComponent(newEntity, shader);
	world.AddComponent(newEntity, transform);
	
	engine.Run();
}
