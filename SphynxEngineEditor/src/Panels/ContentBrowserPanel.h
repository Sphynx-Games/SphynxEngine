#pragma once

#include "Base/Panel.h"
#include "Math/Vector.h"

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
		bool RenderImageButtonWithText(const std::string& text, const class Texture* texture, Vector2f size);

	private:
		std::filesystem::path m_CurrentDirectory;
		bool m_ShowAllFiles;
	};
}
