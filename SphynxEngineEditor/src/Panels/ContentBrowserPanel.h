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
		bool RenderContentItem(const int numItem, const std::string& text, const class Texture* texture, Vector2f size, Color color = Color::White);
		bool RenderRenamingContentItem(const std::filesystem::path& path, const int numItem, const std::string& text, const class Texture* texture, Vector2f size, Color color = Color::White);
		
		void RenderRenameOption(const std::filesystem::path& path);
		void RenderDeleteOption(const std::filesystem::path& path);
		void RenderContentItemCommonOptions(const std::filesystem::path& path);

		void RenderContentItemContextMenu(const std::string& filename, const std::filesystem::path& path);
		void RenderContentItemAssetContextMenu(const std::string& filename, const std::filesystem::path& path);
		
		bool IsRenaming(const std::filesystem::path& path);

	private:
		std::filesystem::path m_CurrentDirectory;
		bool m_ShowAllFiles;
		int m_SelectedItem;
		std::filesystem::path m_PathToRename;
		char m_RenameBuffer[1024];
	};
}
