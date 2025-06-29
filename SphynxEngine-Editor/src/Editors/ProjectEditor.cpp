#include "spxpch.h"
#include "ProjectEditor.h"

#include "Dialogs/FileDialog.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Asset/AssetManager.h"
#include "Core/Application.h"

#include <imgui.h>


namespace Sphynx
{
	ProjectEditor::ProjectEditor() :
		Editor("ProjectEditor"),
		m_ProjectHandle(ModuleHandle::Invalid)
	{
		for (auto& [key, values] : Application::GetInstance()->GetCommandArguments())
		{
			if (key == "-p" || key == "--project")
			{
				OpenProject(values[0]);
			}
		}
	}

	void ProjectEditor::RenderGUI()
	{
		// This will append menu bar items into the parents menu bar
		// But only if the window has not shown them already
		if (HasMenuBar())
		{
			if (ImGui::BeginMenuBar())
			{
				RenderMenuBar();
				ImGui::EndMenuBar();
			}
		}
	}

	void ProjectEditor::RenderMenuBar()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Project...", nullptr, nullptr))
			{
				auto path = FileDialog::Open();
				if (!path.empty())
				{
					OpenProject(path);
				}
			}

			ImGui::EndMenu();
		}
	}

	void ProjectEditor::OpenProject(const std::filesystem::path& path)
	{
		// Change working directory 
		// TODO: change this platform specific
		std::filesystem::path newCurrentDirectory = path;
		newCurrentDirectory.remove_filename();
#ifdef SPX_PLATFORM_WINDOWS
		SetCurrentDirectory(newCurrentDirectory.string().c_str());
#endif
		
		// Deserialize project file
		ProjectInfo projectInfo = {};

		YAMLReader reader{ path };
		ReflectionDeserializer deserializer{ projectInfo, reader };
		deserializer.Deserialize();

		// Load {projectInfo.Name}.dll and broadcast delegate
		LoadProject(MODULE_PATH(projectInfo.Name));
		OnProjectOpened.Broadcast(projectInfo);
	}

	void ProjectEditor::LoadProject(const std::filesystem::path& path)
	{
		UnloadProject();

		m_ProjectHandle = ModuleManager::LoadModule(path);

		// TODO: delete this line in the future
		AssetManager::Shutdown();
		Reflection::Registry::Shutdown();
		Reflection::Registry::Init();
		AssetManager::Init();
	}

	void ProjectEditor::UnloadProject()
	{
		ModuleManager::UnloadModule(m_ProjectHandle);
		m_ProjectHandle = ModuleHandle::Invalid;
	}
}