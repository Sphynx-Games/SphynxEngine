#pragma once

#include "Base/Panel.h"

#include <filesystem>


namespace Sphynx
{
	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();

	protected:
		virtual void RenderGUI() override;

	private:
		std::filesystem::path m_CurrentDirectory;
		bool m_ShowAllFiles;
	};
}
