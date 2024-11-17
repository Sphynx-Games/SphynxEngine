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
	EditorApplication::EditorApplication()
	{
	}

	void EditorApplication::Init()
	{
		// Init engine resources
		Application::Init();

		Resources::Init();

		// create a editor layer
		EditorLayer* editorLayer = EditorLayer::Create();

		// add gui widgets to the EditorLayer
		editorLayer->AddWidget(new SceneEditor());

		PushLayer(editorLayer);

	}

	void EditorApplication::Run()
	{
		Application::Run();
	}

	void EditorApplication::Shutdown()
	{
		Resources::Shutdown();

		Application::Shutdown();
	}
}
