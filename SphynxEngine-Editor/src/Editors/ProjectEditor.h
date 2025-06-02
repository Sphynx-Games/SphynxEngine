#pragma once

#include "Base/Editor.h"
#include "Module/ModuleManager.h"
#include "Core/Delegate.h"
#include "ProjectInfo.h"

#include <filesystem>


namespace Sphynx
{
	class ProjectEditor : public Editor
	{
	public:
		ProjectEditor();
		virtual ~ProjectEditor() = default;

	protected:
		virtual void RenderGUI() override;

		// Menu bar
		virtual bool HasMenuBar() const override { return true; }
		virtual void RenderMenuBar() override;

	private:
		void OpenProject(const std::filesystem::path& path);
		void LoadProject(const std::filesystem::path& path);
		void UnloadProject();

	public:
		inline static MulticastDelegate<void(const ProjectInfo&)> OnProjectOpened;

	private:
		ModuleHandle m_ProjectHandle;
	};
}