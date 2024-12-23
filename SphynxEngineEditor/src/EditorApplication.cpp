#include "spxpch.h"
#include "EditorApplication.h"
#include "Base/Resources.h"
#include "EditorLayer.h"

#include <Core/EntryPoint.h>


Sphynx::Application* CreateApplication()
{
	return new Sphynx::EditorApplication();
}

namespace Sphynx
{
	EditorApplication::EditorApplication() :
		m_EditorLayer(nullptr)
	{
	}

	void EditorApplication::Init()
	{
		// Init engine resources
		Application::Init();

		Resources::Init();

		// create a editor layer
		m_EditorLayer = new EditorLayer();

		PushLayer(m_EditorLayer);
	}

	void EditorApplication::Run()
	{
		Application::Run();
	}

	void EditorApplication::Shutdown()
	{
		PopLayer(m_EditorLayer);
		delete m_EditorLayer;

		Resources::Shutdown();

		Application::Shutdown();
	}
}
