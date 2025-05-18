#include <iostream>

#include <Sphynx.h>
#include <Core/EntryPoint.h>
#include "Core/Application.h"
#include "Renderer/GraphicsContext.h"
#include "Asset/AssetManager.h"
#include "Scene/SceneRenderer.h"
#include <Windows.h>
#include <tchar.h>


class RuntimeLayer : public Sphynx::Layer
{
public:
	RuntimeLayer(const std::filesystem::path& path) :
		m_PathInitialScene(path),
		m_RuntimeScene()
	{
	}

	virtual void Attach() override;
	virtual void Detach() override;
	virtual void Update(float deltaTime) override;
	virtual void HandleEvent(Sphynx::Event& event) override;

private:
	const std::filesystem::path& m_PathInitialScene;
	Sphynx::Scene m_RuntimeScene;
};


class RuntimeApplication : public Sphynx::Application
{
public:
	RuntimeApplication() : m_GameDLL(NULL)
	{
	}

private:
	HMODULE m_GameDLL;


public:
	virtual void Init() override 
	{
		using namespace Sphynx;

		Application::Init();

		// load Sandbox dll.  TODO: refactor this is platform specific code
		m_GameDLL = LoadLibrary(_T("Sandbox"));
		if (!m_GameDLL || m_GameDLL == INVALID_HANDLE_VALUE)
		{
			std::cerr << "Library Sandbox.dll not found" << std::endl;
			exit(1);
		}

		// TODO: delete this line in the future
		Reflection::Registry::Init();

		using GetPathInitialSceneFunc = const std::filesystem::path& (*)();
		GetPathInitialSceneFunc getPathScene = (GetPathInitialSceneFunc)GetProcAddress(m_GameDLL, "GetPathInitialScene");
		if (!getPathScene) {
			std::cerr << "Failed to find PathInitalScene." << std::endl;
			FreeLibrary(m_GameDLL);
			exit(1);
		}
		
		const std::filesystem::path& path = getPathScene();

		// layers
		m_RuntimeLayer = new RuntimeLayer(path);
		PushLayer(m_RuntimeLayer);
	}

	virtual void Run() override { Sphynx::Application::Run(); }

	virtual void Shutdown() override 
	{ 
		PopLayer(m_RuntimeLayer);
		delete m_RuntimeLayer;
		m_RuntimeLayer = nullptr;

		FreeLibrary(m_GameDLL);

		Sphynx::Application::Shutdown(); 
	}

private:
	RuntimeLayer* m_RuntimeLayer;

};

Sphynx::Application* CreateApplication()
{
	return new RuntimeApplication();
}

void RuntimeLayer::Attach()
{
	using namespace Sphynx;

	YAMLReader reader{ m_PathInitialScene };
	SceneDeserializer sceneDeserializer{ m_RuntimeScene, reader };
	sceneDeserializer.Deserialize();

	m_RuntimeScene.BeginPlay();
}

void RuntimeLayer::Detach()
{
	Sphynx::AssetManager::UnloadAssets();
}

void RuntimeLayer::Update(float deltaTime)
{
	using namespace Sphynx;

	// update camera
	//m_CameraController->Update(deltaTime);

	// begin scene render
	SceneRenderer::Render(m_RuntimeScene);
	m_RuntimeScene.Update(deltaTime);

	if (auto* gContext = Application::GetInstance()->GetWindow()->GetGraphicsContext())
	{
		gContext->SwapBuffers();
	}
}

void RuntimeLayer::HandleEvent(Sphynx::Event& event)
{
	//m_CameraController->HandleEvent(event);
}