#include <iostream>

#include <Sphynx.h>
#include <Core/EntryPoint.h>

Sphynx::Spritesheet* sheet = nullptr;
Sphynx::Texture* enemyTexture = nullptr;

class SandboxLayer : public Sphynx::Layer
{
public:
	SandboxLayer() :
		m_CameraController(new Sphynx::OrthographicCameraController(1.0f, true))
	{}

	virtual void Attach() override;
	virtual void Detach() override;
	virtual void Update(float deltaTime) override;
	virtual void HandleEvent(Sphynx::Event& event) override;

private:
	std::unique_ptr<class Sphynx::OrthographicCameraController> m_CameraController;
	Sphynx::Scene m_SandboxScene;
};


class SandboxApplication : public Sphynx::Application
{
public:
	SandboxApplication()
	{
	}

public:
	virtual void Init() override 
	{
		using namespace Sphynx;

		Application::Init();

		// assets
		AssetManager::Import("..\\Assets\\Textures\\cat.jpg");
		enemyTexture = *AssetManager::Import<Texture>("..\\Assets\\Textures\\enemy_scaled5x.png");
		sheet = new Spritesheet(enemyTexture, 4, 3);

		// layers
		PushLayer(new SandboxLayer());

		SPX_LOG_DISPLAY("1. Esto es un UUID: {}", UUID::ToString(UUID::Generate()));
		SPX_LOG_DISPLAY("2. Esto es un UUID: {}", UUID::ToString(UUID::Generate()));
		UUID uuid = UUID::Generate();
		SPX_LOG_DISPLAY("3. Esto es un UUID igual?: {}", uuid == uuid);

	}

	virtual void Run() override { Sphynx::Application::Run(); }

	virtual void Shutdown() override { Sphynx::Application::Shutdown(); }
};

Sphynx::Application* CreateApplication()
{
	return new SandboxApplication();
}

void SandboxLayer::Attach()
{
	using namespace Sphynx;
	Actor sprt = m_SandboxScene.CreateActor();
	sprt.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	sprt.AddComponent<SpriteRendererComponent>(sheet->GetSprite(0), Color::Blue);

	Actor quad = m_SandboxScene.CreateActor();
	quad.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	quad.AddComponent<BoxRendererComponent>();

	Actor line = m_SandboxScene.CreateActor();
	line.AddComponent<TransformComponent>(Transform{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 2.0f, 1.0f }, { 0.0f, 0.0f, 45.0f } });
	line.AddComponent<LineRendererComponent>();
}

void SandboxLayer::Detach()
{
}

void SandboxLayer::Update(float deltaTime)
{
	using namespace Sphynx;

	// update camera
	m_CameraController->Update(deltaTime);

	// begin scene render
	Renderer2D::Begin(&m_CameraController->GetCamera());
	{
		m_SandboxScene.Update(deltaTime);
	}
	Renderer2D::End();
}

void SandboxLayer::HandleEvent(Sphynx::Event& event)
{
	m_CameraController->HandleEvent(event);
}