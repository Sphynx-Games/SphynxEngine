#include <iostream>

#include <Sphynx.h>


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
		Assets::AddTexture("..\\Assets\\Textures\\cat.jpg");
		enemyTexture = Assets::AddTexture("..\\Assets\\Textures\\enemy_scaled5x.png");
		sheet = new Spritesheet(enemyTexture, 4, 3);

		// layers
		PushLayer(new SandboxLayer());
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

	Actor quad = m_SandboxScene.CreateActor();
	quad.AddComponent<TransformComponent>(Transform{ { 200.0f, 300.0f, 0.0f }, { 200.0f, 100.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
	//quad.AddComponent<SpriteRendererComponent>(nullptr, Color::Blue);
	quad.AddComponent<SpriteRendererComponent>(sheet->GetSprite(0), Color::Blue);
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

		/* World space draw commands tests */
		Transform triangleTransform = { { 100.0f, 125.0f, 0.0f }, { 5.0f, 10.0f, 1.0f }, { 0.0f, 0.0f, 180.0f } };
		Renderer2D::DrawTriangle(DrawMode::FILLED, triangleTransform, Vector2f{ -50.0f, -25.0f }, Vector2f{ 0.0f, 25.0f }, Vector2f{ 50.f, -25.0f }, Vector2f{ 0.5f, 1.0f }, Color::Red);

		Transform circleTransform = { { 250.0f, 125.0f, 0.0f }, { 5.5f, 1.5f, 1.0f }, { 0.0f, 0.0f, 90.0f } };
		Renderer2D::DrawCircle(DrawMode::FILLED, circleTransform, 10.f, 32, { 1.0f, 1.0f }, Color::Green);

		/* Screen-space draw commands tests */
		Renderer2D::DrawPoint({ 0, 0 }, Color::Red);
		Renderer2D::DrawLine({ 0,0 }, { 100,50 }, Color{ 206, 16, 236, 255 });
		Renderer2D::DrawQuad({ 300, 600 }, { 1280 / 2, 720 / 2 }, Color::Red);
		Renderer2D::DrawTriangle(DrawMode::WIREFRAME, Vector2i{ 50, 50 }, Vector2i{ 100, 50 }, Vector2i{ 10, 0 }, Color::Red);
		//Renderer2D::DrawCircle(DrawMode::WIREFRAME, { 400,400 }, 200, 50, trans, { 206, 255, 236, 255 });

		static float rot = 0.0f;
		rot += 5*deltaTime;
		Transform textureTransform = { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, rot, 0.0f } };
		Texture* texture = Sphynx::Assets::GetTexture("..\\Assets\\Textures\\cat.jpg");
		/*if (texture != nullptr)
		{
			Renderer2D::DrawTexture(*texture, textureTransform, { 600, 600 });
		}*/

		Renderer2D::DrawSprite(*sheet->GetSprite(6), {600, 600});
		Renderer2D::DrawSprite(*sheet->GetSprite(0), textureTransform, {600, 600});
	}
	Renderer2D::End();
}

void SandboxLayer::HandleEvent(Sphynx::Event& event)
{
	m_CameraController->HandleEvent(event);
}