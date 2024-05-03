#include <iostream>

#include <Sphynx.h>
#include <Core/EntryPoint.h>

Sphynx::Spritesheet* sheet = nullptr;
Sphynx::Texture* enemyTexture = nullptr;

class SandboxLayer : public Sphynx::Layer
{
public:
	SandboxLayer() :
		m_CameraController(new Sphynx::OrthographicCameraController(16.0f/9.0f, true))
	{
		m_CameraController->SetZoom(2.0f);
	}

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
	line.AddComponent<TransformComponent>(Transform{ { 0.5f, -1.5f, 0.0f }, { 5.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	line.AddComponent<LineRendererComponent>(Vector2f{ 0.0f, 0.0f }, Vector2f{ 1.0f, 0.0f }, 1.0f, Color::Green); // TODO: fix line width

	Actor staticRigidBody = m_SandboxScene.CreateActor();
	staticRigidBody.AddComponent<TransformComponent>(Transform{ { 0.0f, -1.5f, 0.0f }, { 5.0f, 0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	staticRigidBody.AddComponent<RigidbodyComponent>();
	staticRigidBody.AddComponent<BoxCollider2DComponent>();

	/*Actor dynamicRigidbody = m_SandboxScene.CreateActor();
	dynamicRigidbody.AddComponent<TransformComponent>(Transform{ { 0.5f, 3.0f, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 35.0f } });
	dynamicRigidbody.AddComponent<RigidbodyComponent>(RigidbodyType::DYNAMIC);
	dynamicRigidbody.AddComponent<BoxCollider2DComponent>();

	Actor dynamicSphereRigidbody = m_SandboxScene.CreateActor();
	dynamicSphereRigidbody.AddComponent<TransformComponent>(Transform{ { 0.5f, 3.0f, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 35.0f } });
	dynamicSphereRigidbody.AddComponent<RigidbodyComponent>(RigidbodyType::DYNAMIC);
	dynamicSphereRigidbody.AddComponent<CircleCollider2DComponent>();*/

	Actor capsuleRigidbody = m_SandboxScene.CreateActor();
	capsuleRigidbody.AddComponent<TransformComponent>(Transform{ { 1.0f, 1.0f, 0.0f }, { 1.5f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	//capsuleRigidbody.AddComponent<TransformComponent>(Transform{ { 0.0f, 1.0f, 0.0f }, { 2.0f, -0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f } }); // TODO: take into account negative sizes
	//capsuleRigidbody.AddComponent<TransformComponent>(Transform{ { 0.0f, 1.0f, 0.0f }, { 2.0f, 2.0f, 1.0f }, { 0.0f, 0.0f, -10.0f } });
	capsuleRigidbody.AddComponent<RigidbodyComponent>(RigidbodyType::DYNAMIC);
	capsuleRigidbody.AddComponent<CapsuleCollider2DComponent>();

	m_SandboxScene.BeginPlay();
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