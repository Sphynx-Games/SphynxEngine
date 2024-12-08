#include "spxpch.h"
#include "EditorApplication.h"
#include "Base/Resources.h"
#include "EditorLayer.h"
#include "Editors/SceneEditor.h"

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

		// add gui widgets to the EditorLayer
		m_EditorLayer->AddWidget(new SceneEditor());

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
