#include "spxpch.h"
#include "ContentBrowserPanel.h"
#include "Base/Resources.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/TextureAsset.h"
#include "Asset/Sprite/SpriteAsset.h"
#include "Dialogs/FileDialog.h"
#include "Renderer/Texture.h"
#include "Events/Event.h"
#include "Events/InputEvent.h"
#include "Input/Keycode.h"
#include "ImGuiExtra.h"

#include <imgui.h>


namespace Sphynx
{
	static const std::filesystem::path s_AssetsPath = "Assets";

	struct ContentItem
	{
	public:
		ContentItem() = default;

		ContentItem(const std::filesystem::path& absolutePath, const int& number = -1) :
			Number(number),
			AbsolutePath(absolutePath),
			RelativePath(std::filesystem::relative(AbsolutePath, s_AssetsPath)),
			Filename(RelativePath.filename().string())
		{}

		void CopyValues(const ContentItem& item)
		{
			Number = item.Number;
			AbsolutePath = item.AbsolutePath;
			RelativePath = item.RelativePath;
			Filename = item.Filename;
		}

		void ResetValues()
		{
			Number = -1;
			AbsolutePath = "";
			RelativePath = "";
			Filename = "";
		}

	public:
		int Number = -1;
		std::filesystem::path AbsolutePath;
		std::filesystem::path RelativePath;
		std::string Filename;
	};


	std::filesystem::path MakeRelativePath(const std::filesystem::path& absolutePath)
	{
		// Get the parent path up to the target folder
		std::filesystem::path currentPath = absolutePath;

		while (currentPath.has_parent_path())
		{
			if (currentPath.filename() == s_AssetsPath)
			{
				// If we find the target folder, return the relative path from it
				return std::filesystem::path(s_AssetsPath) / std::filesystem::relative(absolutePath, currentPath);
			}
			currentPath = currentPath.parent_path();
		}

		return std::filesystem::path(); // Return an empty path if the target folder is not found
	}


	ContentBrowserPanel::ContentBrowserPanel(Widget* parent) :
		Panel("Content Browser", parent),
		m_CurrentDirectory(s_AssetsPath),
		m_ShowAllFiles(false),
		m_SelectedContentItem(new ContentItem()),
		m_PathToRename(),
		m_RenameBuffer()
	{
		std::fill(std::begin(m_RenameBuffer), std::end(m_RenameBuffer), '\0');
	}

	void ContentBrowserPanel::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
			{
				if (e.GetKeycode() == SPX_KEY_DELETE)
				{
					DeleteContentItem(m_SelectedContentItem->AbsolutePath);
				}

				return false;
			});
	}

	void ContentBrowserPanel::RenderGUI()
	{
		ImGui::Begin(GetName());

		// BACK BUTTON
		ImGui::BeginDisabled(m_CurrentDirectory == s_AssetsPath);
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
			m_SelectedContentItem->ResetValues();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		float itemPadding = ImGui::GetStyle().ItemSpacing.x;
		float itemWidth1 = ImGui::CalcTextSize("Import").x + itemPadding * 2.f;
		float itemWidth2 = ImGui::CalcTextSize("Show all").x + itemPadding * 2.f;
		float widthNeeded = itemWidth1 + itemPadding + itemWidth2;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);

		// IMPORT BUTTON
		if (ImGui::Button("Import"))
		{
			std::filesystem::path path = FileDialog::Open();
			if (!path.empty())
			{
				AssetManager::Import(MakeRelativePath(path));
			}
		}

		ImGui::SameLine();

		// SHOW ALL FILES CHECKBOX
		ImGui::Checkbox("Show all", &m_ShowAllFiles);

		// CONTENT
		int numItems = 0;
		Vector2f sizeItem{ 85.0f, 85.0f };
		float window_visible_x2 = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			++numItems;
			ImGui::PushID("ContentItem_" + numItems);

			// apply SameLine if we are not printing the first item and if it fits into the remaining windows space
			float last_item_x2 = ImGui::GetItemRectMax().x;
			float next_item_x2 = last_item_x2 + ImGui::GetStyle().ItemSpacing.x + sizeItem.X; // Expected position if next item was on same line
			if (numItems != 1 && next_item_x2 < window_visible_x2)
			{
				ImGui::SameLine();
			}

			//ImVec2 pos = ImGui::GetCursorScreenPos();
			//ImVec2 marker_min = ImVec2(pos.x + sizeItem.X, pos.y);
			//ImVec2 marker_max = ImVec2(pos.x + sizeItem.X + 10, pos.y + ImGui::GetTextLineHeight());

			// get the name of the current item
			ContentItem currentItem(directoryEntry.path(), numItems);
			
			// render directories
			if (directoryEntry.is_directory())
			{
				if(!RenderRenamingContentItem(currentItem, Resources::FolderTexture, sizeItem))
				{
					if (RenderContentItem(currentItem, Resources::FolderTexture, sizeItem))
					{
						m_CurrentDirectory /= currentItem.Filename;
						m_SelectedContentItem->ResetValues();
					}
					RenderContentItem_ContextMenu(currentItem);
				}
			}
			else // render files
			{				
				if (m_ShowAllFiles)
				{
					if (!RenderRenamingContentItem(currentItem, Resources::FileTexture, sizeItem))
					{
						RenderContentItem(currentItem, Resources::FileTexture, sizeItem);
						RenderContentItem_ContextMenu(currentItem);
					}
				}
				else
				{
					if (currentItem.RelativePath.extension() == ASSET_EXTENSION)
					{
						if (!RenderRenamingContentItem(currentItem, Resources::FileTexture, sizeItem))
						{
							RenderContentItem(currentItem, Resources::FileTexture, sizeItem);
							RenderContentItem_AssetContextMenu(currentItem);
						}
					}
				}
			}

			ImGui::PopID();
		}

		// NEW FOLDER CONTEXT ITEM OPTION
		if (ImGui::BeginPopupContextWindow("New folder", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
		{
			if (ImGui::MenuItem("New folder"))
			{
				if(!std::filesystem::is_directory("src") || !std::filesystem::exists("src"))
				{
					std::filesystem::path folder = m_CurrentDirectory / "New folder";
					std::filesystem::create_directory(folder.c_str());
				}
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	bool ContentBrowserPanel::RenderContentItem(const ContentItem& contentItem, const Texture* texture, Vector2f size, Color color)
	{
		ImVec2 buttonSize{ size.X, size.Y };
		ImVec4 tintColor{ color.R / 255.f, color.G / 255.f, color.B / 255.f, color.A / 255.f };

		bool pressed = ImGui::SelectableItemWithImageAndText(
			m_SelectedContentItem->Number,
			contentItem.Number,
			contentItem.Filename,
			(ImTextureID)texture->GetNativeTexture(),
			buttonSize,
			tintColor,
			ImGuiSelectableFlags_AllowDoubleClick
		);

		// if item was clicked => save values of the new current selected item
		if (m_SelectedContentItem->Number == contentItem.Number)
		{
			m_SelectedContentItem->CopyValues(contentItem);
		}

		return pressed;
	}

	bool ContentBrowserPanel::RenderRenamingContentItem(const ContentItem& contentItem, const Texture* texture, Vector2f size, Color color)
	{
		if (!IsRenaming(contentItem.AbsolutePath)) return false;

		ImVec2 buttonSize{ size.X, size.Y };
		ImVec4 tintColor{ color.R / 255.f, color.G / 255.f, color.B / 255.f, color.A / 255.f };

		ImGui::SelectableItemWithImageAndTextInput(
			m_SelectedContentItem->Number,
			contentItem.Number,
			contentItem.Filename,
			m_RenameBuffer,
			sizeof(m_RenameBuffer) / sizeof(char),
			(ImTextureID)texture->GetNativeTexture(),
			buttonSize,
			tintColor,
			ImGuiSelectableFlags_AllowDoubleClick
		);

		if (ImGui::IsItemDeactivated())
		{
			if (m_RenameBuffer[0] != '\0')
			{
				std::filesystem::path newPathName = m_PathToRename;
				std::filesystem::path extension = newPathName.extension();

				newPathName.remove_filename();
				newPathName += std::string(m_RenameBuffer);
				newPathName.replace_extension(extension);

				std::filesystem::rename(m_PathToRename, newPathName);

				std::fill(std::begin(m_RenameBuffer), std::end(m_RenameBuffer), '\0');
			}

			m_PathToRename = "";
		}

		return true;
	}

	void ContentBrowserPanel::RenderContentItem_ContextMenu(const ContentItem& contentItem)
	{
		if (ImGui::BeginPopupContextItem(contentItem.Filename.c_str()))
		{
			m_SelectedContentItem->CopyValues(contentItem);

			RenderContentItem_CommonOptions(contentItem.AbsolutePath);
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::RenderContentItem_AssetContextMenu(const ContentItem& contentItem)
	{
		const AssetMetadata& metadata = AssetManager::GetAssetMetadataFromPath(s_AssetsPath / contentItem.RelativePath);

		if (ImGui::BeginPopupContextItem(contentItem.Filename.c_str()))
		{
			m_SelectedContentItem->CopyValues(contentItem);

			if (metadata.Type == TypeToAssetType<Texture>::Value)
			{
				// "Create sprite" for .spxasset of Texture type
				if (ImGui::MenuItem("Create sprite"))
				{
					AssetMetadata spriteMetadata;
					spriteMetadata.Handle = AssetHandle::Generate();
					spriteMetadata.Type = TypeToAssetType<Sprite>::Value;

					std::filesystem::path name = metadata.Path.filename();
					name.replace_extension();
					name += "_sprite";
					name.replace_extension(ASSET_EXTENSION);

					spriteMetadata.Path = metadata.Path;
					spriteMetadata.Path.replace_filename(name);

					spriteMetadata.Dependencies.Add(metadata.Handle);

					AssetImporter::Save(spriteMetadata);
					AssetManager::AddToRegistry(spriteMetadata);

					ImGui::CloseCurrentPopup();
				}
			}

			RenderContentItem_CommonOptions(contentItem.RelativePath);
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::RenderContentItem_CommonOptions(const std::filesystem::path& path)
	{
		RenderContentItem_RenameOption(path);
		RenderContentItem_DeleteOption(path);
	}

	void ContentBrowserPanel::RenderContentItem_RenameOption(const std::filesystem::path& path)
	{
		if (ImGui::MenuItem("Rename"))
		{
			m_PathToRename = path;
		}
	}

	void ContentBrowserPanel::RenderContentItem_DeleteOption(const std::filesystem::path& path)
	{
		if (ImGui::MenuItem("Delete"))
		{
			DeleteContentItem(path);
		}
	}

	bool ContentBrowserPanel::IsRenaming(const std::filesystem::path& path)
	{
		return !m_PathToRename.string().empty() && m_PathToRename == path;
	}

	void ContentBrowserPanel::DeleteContentItem(const std::filesystem::path& path)
	{
		std::filesystem::remove_all(path);
		m_SelectedContentItem->ResetValues();
	}
}
