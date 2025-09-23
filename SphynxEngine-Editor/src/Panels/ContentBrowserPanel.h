#pragma once

#include "Base/Panel.h"
#include <Math/Vector.h>
#include <Math/Color.h>
#include <Core/Delegate.h>

#include <filesystem>


namespace Sphynx
{
	namespace Reflection
	{
		struct Class;
	}
	struct ContentItem;
	struct AssetMetadata;
	class Prefab;

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel(Widget* parent = nullptr);

	protected:
		virtual void HandleEvent(class Event& event) override;

		virtual void RenderGUI() override;

	private:
		bool RenderContentItem(const ContentItem& contentItem, const class Texture* texture, Vector2f size, Color color = Color::White);
		bool RenderRenamingContentItem(const ContentItem& contentItem, const class Texture* texture, Vector2f size, Color color = Color::White);
		
		void RenderContentItem_ContextMenu(const ContentItem& contentItem);
		void RenderContentItem_AssetContextMenu(const ContentItem& contentItem);

		void RenderContentItem_CreateSpriteOption(const AssetMetadata& metadata);
		void RenderContentItem_EditOption(const AssetMetadata& metadata);
		
		void RenderContentItem_CommonOptions(const std::filesystem::path& path);
		void RenderContentItem_RenameOption(const std::filesystem::path& path);
		void RenderContentItem_DeleteOption(const std::filesystem::path& path);

		bool IsRenaming(const std::filesystem::path& path);
		void DeleteContentItem(const std::filesystem::path& path);

	public:
		inline static MulticastDelegate<void(Prefab*)> OnPrefabEdit;
		inline static MulticastDelegate<void(const Reflection::Class&, void*)> OnGenericAssetEdit;

	private:
		std::filesystem::path m_CurrentDirectory;
		bool m_ShowAllFiles;
		ContentItem* m_SelectedContentItem;
		std::filesystem::path m_PathToRename;
		char m_RenameBuffer[1024];
	};
}
