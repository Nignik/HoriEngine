#include <World.h>
#include <Core/Sprite.h>
#include <Core/HoriEngine.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>
#include <Core/Controller.h>
#include <Core/VelocityComponent.h>
#include <Core/Collider.h>

#include <gtest/gtest.h>

TEST(TestSprite, RenderingSprites)
{
	auto& engine = Hori::Engine::GetInstance();
	engine.InitSystems();

	auto& renderer = Hori::Renderer::GetInstance();
	auto& world = Hori::World::GetInstance();

	auto player = world.CreateEntity();

	Hori::Transform transform = {
		.position = {200.0f, 200.0f},
		.rotation = 0.0f,
		.scale = {200.0f, 200.0f}
	};
	Hori::Shader shader = Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs");
	Hori::Texture2D texture = Hori::LoadTextureFromFile("resources/sprites/awesomeface.png", true);
	
	world.AddComponent(player, Hori::Sprite());
	world.AddComponent(player, Hori::Controller());
	world.AddComponent(player, Hori::VelocityComponent({0.0f, 0.0f}, 500.0f));
	world.AddComponent(player, Hori::SphereCollider(Hori::Transform({ 300.0f, 300.0f }, 0.0f, { 50.0f, 50.0f }), 100.0f));
	world.AddComponent(player, shader);
	world.AddComponent(player, transform);
	world.AddComponent(player, texture);

	const auto& box = world.CreateEntity();
	world.AddComponent(box, Hori::Sprite());
	world.AddComponent(box, Hori::Transform({ 50.0f, 50.0f }, 0.0f, { 50.0f, 50.0f }));
	world.AddComponent(box, Hori::SphereCollider(Hori::Transform({ 75.0f, 75.0f }, 0.0f, { 50.0f, 50.0f }), 25.0f));
	world.AddComponent(box, shader);
	world.AddComponent(box, texture);

	/*const auto& col = world.CreateEntity();
	world.AddComponent(col, Hori::Sprite());
	world.AddComponent(col, Hori::Transform({ 50.0f, 50.0f }, 0.0f, { 100.0f, 100.0f }));
	world.AddComponent(col, shader);
	world.AddComponent(col, texture);*/
	
	engine.Run();
}
