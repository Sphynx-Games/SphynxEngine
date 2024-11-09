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
		
		void RenderContentItem_ContextMenu(const ContentItem& contentItem);
		void RenderContentItem_AssetContextMenu(const ContentItem& contentItem);
		
		void RenderContentItem_CommonOptions(const std::filesystem::path& path);
		void RenderContentItem_RenameOption(const std::filesystem::path& path);
		void RenderContentItem_DeleteOption(const std::filesystem::path& path);

		bool IsRenaming(const std::filesystem::path& path);
		void DeleteContentItem(const std::filesystem::path& path);

	private:
		std::filesystem::path m_CurrentDirectory;
		bool m_ShowAllFiles;
		ContentItem* m_SelectedContentItem;
		std::filesystem::path m_PathToRename;
		char m_RenameBuffer[1024];
	};
}
