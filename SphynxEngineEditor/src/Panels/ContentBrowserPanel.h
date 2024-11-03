#pragma once

#include "Base/Panel.h"
#include "Math/Vector.h"
#include "Math/Color.h"

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
		bool RenderSelectableItemWithImageAndText(const int numItem, const std::string& text, const class Texture* texture, Vector2f size, Color color = Color::White);

	private:
		std::filesystem::path m_CurrentDirectory;
		bool m_ShowAllFiles;
		int m_SelectedItem;
	};
}
