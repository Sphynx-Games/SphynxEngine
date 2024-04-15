#include "EditorApplication.h"

#include "EditorLayer.h"
#include "Editors/SceneEditor.h"


Sphynx::Application* CreateApplication()
{
	return new EditorApplication();
}

EditorApplication::EditorApplication()
{
}

void EditorApplication::Init()
{
	Sphynx::Application::Init();
	
	// create a editor layer
	EditorLayer* editorLayer = EditorLayer::Create();

	// add gui widgets to the EditorLayer
	editorLayer->AddWidget(new SceneEditor());

	PushLayer(editorLayer);

}

void EditorApplication::Run()
{
	Sphynx::Application::Run();
}

void EditorApplication::Shutdown()
{
	Sphynx::Application::Shutdown();
}
