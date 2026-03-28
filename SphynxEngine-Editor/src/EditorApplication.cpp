#include "spxpch.h"
#include "EditorApplication.h"
#include "Base/Resources.h"
#include "EditorLayer.h"
#include "PropertyDrawer/PropertyDrawerManager.h"
#include "Editors/AssetEditor.h"

#include <Core/EntryPoint.h>
#include <Command/CommandManager.h>
#include <Reflection/PropertyComparator.h>
#include <Math/Vector.h>
#include <Container/Array.h>


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

	EditorApplication* EditorApplication::GetInstance()
	{
		return static_cast<EditorApplication*>(Application::GetInstance());
	}

	void EditorApplication::Init(const HashMap<std::string, Array<std::string>>& commandArguments)
	{
		// Init engine resources
		Application::Init(commandArguments);

		Resources::Init();

		PropertyDrawerManager::Init();

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

		PropertyDrawerManager::Shutdown();

		Resources::Shutdown();

		Application::Shutdown();
	}

	void EditorApplication::OpenAssetEditor(const std::string& name, const AssetMetadata& metadata)
	{
		m_EditorLayer->GetAssetEditor()->OpenAssetEditor(name, metadata, m_EditorLayer);
	}

	OperationManager* EditorApplication::GetOperationManager() const
	{
		return m_EditorLayer->GetCurrentOperationManager();
	}

}
