#include <Sprite.h>
#include <World.h>
#include <Sprite.h>
#include <HoriEngine.h>
#include <Texture.h>
#include <ResourceManager.h>

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
		.scale = {1.0f, 1.0f}
	};
	Hori::Sprite sprite;
	Hori::Shader shader = Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs");

	//std::shared_ptr<Hori::Texture2D> texture = Hori::LoadTextureFromFile("xd", false);
	
	world.AddComponent(newEntity, sprite);
	world.AddComponent(newEntity, shader);
	world.AddComponent(newEntity, transform);
	
	engine.Run();
}
