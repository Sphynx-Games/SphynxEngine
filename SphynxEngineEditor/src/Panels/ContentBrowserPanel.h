#pragma once

#include "Base/Panel.h"
#include "Math/Vector.h"
#include "Math/Color.h"

#include <filesystem>


namespace Sphynx
{
	struct ContentItem;

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();

	protected:
		virtual void RenderGUI() override;

	private:
		void HandleEvent(class Event& event);

		bool RenderContentItem(const ContentItem& contentItem, const class Texture* texture, Vector2f size, Color color = Color::White);
		bool RenderRenamingContentItem(const ContentItem& contentItem, const class Texture* texture, Vector2f size, Color color = Color::White);
		
		void RenderRenameOption(const std::filesystem::path& path);
		void RenderDeleteOption(const std::filesystem::path& path);
		void RenderContentItemCommonOptions(const std::filesystem::path& path);

		void RenderContentItemContextMenu(const ContentItem& contentItem);
		void RenderContentItemAssetContextMenu(const ContentItem& contentItem);
		
		bool IsRenaming(const std::filesystem::path& path);
		void ResetSelectedContentItem();

	private:
		std::filesystem::path m_CurrentDirectory;
		bool m_ShowAllFiles;

		//ContentItem& m_SelectedContentItem;

		int m_SelectedItem;
		std::filesystem::path m_PathToRename;
		char m_RenameBuffer[1024];
	};
}
