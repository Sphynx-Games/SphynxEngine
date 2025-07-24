#include "spxpch.h"
#include "EditorApplication.h"
#include "Base/Resources.h"
#include "EditorLayer.h"

#include <Core/EntryPoint.h>
#include <Command/CommandManager.h>
#include "PropertyDrawer/PropertyDrawerManager.h"


#include "Reflection/PropertyComparator.h"
#include "Math/Vector.h"
#include "Container/Array.h"


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

	void EditorApplication::Init(const HashMap<std::string, Array<std::string>>& commandArguments)
	{
		// Init engine resources
		Application::Init(commandArguments);

		Resources::Init();

		PropertyDrawerManager::Init();

		// create a editor layer
		m_EditorLayer = new EditorLayer();

		PushLayer(m_EditorLayer);


		Map<int, std::string> source;
		source[3] = "JA";
		source[5] = "Hola";

		Map<int, std::string> target;
		target[3] = "JA";
		target[4] = "Holade";

		/*Transform source{{1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}};
		Transform target{ { 1.0f, -200.0f, 3.0f }, { 1.0f, 2.0f, 3.0f }, { 1.0f, 2.0f, 3.0f } };*/

		Reflection::PropertyComparator comparator{ &target, Reflection::GetClass<decltype(target)>() };

		Reflection::PropertyTree::Traverse(Reflection::GetClass<decltype(source)>(), &source, comparator);
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
}
