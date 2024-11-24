#include <iostream>

#include <Sphynx.h>
#include <Core/EntryPoint.h>


std::shared_ptr<Sphynx::Asset<Sphynx::Font>> font = nullptr;
std::shared_ptr<Sphynx::Asset<Sphynx::Spritesheet>> sheet = nullptr;
std::shared_ptr<Sphynx::Asset<Sphynx::Texture>> enemyTexture = nullptr;


class SandboxLayer : public Sphynx::Layer
{
public:
	SandboxLayer() :
		m_CameraController(new Sphynx::OrthographicCameraController(16.0f / 9.0f, true)),
		m_SandboxScene(Sphynx::Scene("TestScene"))
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


#if 0
		// assets
		font = AssetManager::Import<Font>("Assets\\Fonts\\roboto\\Roboto-Regular.ttf");
		AssetManager::Import("Assets\\Textures\\cat.jpg");
		enemyTexture = AssetManager::Import<Texture>("Assets\\Textures\\enemy_scaled5x.png");
		sheet = AssetManager::RegisterAsset<Spritesheet>(Spritesheet::Create(enemyTexture->Asset, 4, 3), "Assets\\Textures\\enemySpritesheet");

		/*Sprite* sprite = new Sprite(enemyTexture->Asset);
		AssetManager::RegisterAsset(sprite, "Assets\\Textures\\enemySprite");*/

		//enemyTexture = AssetManager::GetAsset<Texture>(AssetHandle::FromString("3d8020bb-a60b-4009-bec8-65be84888a39"));
#endif

		// layers
		PushLayer(new SandboxLayer());

		SPX_LOG_DISPLAY("1. Esto es un UUID: {}", UUID::ToString(UUID::Generate()));
		SPX_LOG_DISPLAY("2. Esto es un UUID: {}", UUID::ToString(UUID::Generate()));
		UUID uuid = UUID::Generate();
		SPX_LOG_DISPLAY("3. Esto es un UUID igual?: {}", uuid == uuid);
	}

	virtual void Run() override { Sphynx::Application::Run(); }

	virtual void Shutdown() override 
	{ 
		font.reset();
		sheet.reset();
		enemyTexture.reset();

		Sphynx::Application::Shutdown(); 
	}
};

Sphynx::Application* CreateApplication()
{
	return new SandboxApplication();
}

void SandboxLayer::Attach()
{
	using namespace Sphynx;
#if 1
	Actor& sprt = m_SandboxScene.CreateActor();
	sprt.AddComponent<NameComponent>("Sprite");
	sprt.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	AssetHandle handle = AssetManager::GetAssetHandleFromAddress(sheet->Asset->GetSprite(0));
	sprt.AddComponent<SpriteRendererComponent>(handle, Color::Blue);

	Actor& quad = m_SandboxScene.CreateActor();
	quad.AddComponent<NameComponent>("Quad");
	quad.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	quad.AddComponent<BoxRendererComponent>();

	Actor& line = m_SandboxScene.CreateActor();
	line.AddComponent<NameComponent>("Line");
	//line.AddComponent<TransformComponent>(Transform{ { 0.5f, -1.5f, 0.0f }, { 5.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	line.AddComponent<TransformComponent>(Transform{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	line.AddComponent<LineRendererComponent>(Vector2f{ 0.0f, 0.0f }, Vector2f{ 1.0f, 0.0f }, 1.0f, Color::Green);

	Actor& staticRigidBody = m_SandboxScene.CreateActor();
	staticRigidBody.AddComponent<NameComponent>("Box 1");
	staticRigidBody.AddComponent<TransformComponent>(Transform{ { 0.0f, -2.0f, 0.0f }, { 5.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	//staticRigidBody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::STATIC);
	staticRigidBody.AddComponent<BoxCollider2DComponent>(Vector2f{ 1.0f, 1.0f }, Vector2f{ 0.0f, 1.0f });

	Actor& dynamicRigidbody = m_SandboxScene.CreateActor();
	dynamicRigidbody.AddComponent<NameComponent>("Box 2");
	dynamicRigidbody.AddComponent<TransformComponent>(Transform{ { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 45.0f } });
	dynamicRigidbody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::DYNAMIC);
	dynamicRigidbody.AddComponent<BoxCollider2DComponent>(Vector2f{ 1.0f, 1.0f }, Vector2f{ 0.0f, 0.0f });

	Actor& dynamicSphereRigidbody = m_SandboxScene.CreateActor();
	dynamicSphereRigidbody.AddComponent<NameComponent>("Sphere 1");
	dynamicSphereRigidbody.AddComponent<TransformComponent>(Transform{ { 0.5f, 3.0f, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 35.0f } });
	dynamicSphereRigidbody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::DYNAMIC);
	dynamicSphereRigidbody.AddComponent<CircleCollider2DComponent>(1.0f, Vector2f{ 0.0f, 0.0f });

	Actor& capsuleRigidbody = m_SandboxScene.CreateActor();
	capsuleRigidbody.AddComponent<NameComponent>("Capsule 1");
	//capsuleRigidbody.AddComponent<TransformComponent>(Transform{ { 0.0f, 1.0f, 0.0f }, { 2.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	//capsuleRigidbody.AddComponent<TransformComponent>(Transform{ { 0.0f, 1.0f, 0.0f }, { 2.0f, -0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f } }); // TODO: take into account negative sizes
	capsuleRigidbody.AddComponent<TransformComponent>(Transform{ { -1.5f, 1.0f, 0.0f }, { 2.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, -10.0f } });
	capsuleRigidbody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::DYNAMIC);
	capsuleRigidbody.AddComponent<CapsuleCollider2DComponent>(Vector2f{ 1.0f, 2.0f }, Vector2f{ 1.5f, 0.0f });

	YAMLWriter writer{ "Assets\\Scenes\\" + m_SandboxScene.GetName() + ".sxpasset" };
	SceneSerializer sceneSerializer{ m_SandboxScene, writer };
	sceneSerializer.Serialize();
#else
	YAMLWriter reader{ "Assets\\Scenes\\TestScene.sxpasset" };
	SceneDeserializer sceneDeserializer{ m_SandboxScene, reader };
	sceneDeserializer.Deserialize();
#endif

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
		//Renderer2D::DrawText("hello!!", *font->Asset, 16, { 2,2 });
		m_SandboxScene.Update(deltaTime);
	}
	Renderer2D::End();
}

void SandboxLayer::HandleEvent(Sphynx::Event& event)
{
	m_CameraController->HandleEvent(event);
}