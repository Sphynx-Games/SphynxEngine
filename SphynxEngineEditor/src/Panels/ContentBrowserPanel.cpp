#include "spxpch.h"
#include "ContentBrowserPanel.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/TextureAsset.h"
#include "Asset/Sprite/SpriteAsset.h"
#include "Dialogs/FileDialog.h"
#include "Renderer/Texture.h"
#include "ImGuiExtra.h"

#include <imgui.h>

#include <Windows.h>
#include <commdlg.h>


namespace Sphynx
{
	static const std::filesystem::path s_ResourcesPath = "SphynxEngineEditor\\Resources";

	static const std::filesystem::path s_AssetsPath = "Assets";

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


	ContentBrowserPanel::ContentBrowserPanel() : 
		m_CurrentDirectory(s_AssetsPath),
		m_ShowAllFiles(false),
		m_SelectedItem(-1),
		m_PathToRename(),
		m_RenameBuffer()
	{
		std::fill(std::begin(m_RenameBuffer), std::end(m_RenameBuffer), '\0');
	}

	void ContentBrowserPanel::RenderGUI()
	{
		static Sphynx::Texture* folderTexture = Sphynx::TextureLoader::Load(s_ResourcesPath / "folder.png");
		static Sphynx::Texture* fileTexture = Sphynx::TextureLoader::Load(s_ResourcesPath / "file.png");


		ImGui::Begin("Content Browser");

		// BACK BUTTON
		ImGui::BeginDisabled(m_CurrentDirectory == s_AssetsPath);
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
			m_SelectedItem = -1;
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
		int selectedItem = -1;
		Vector2f sizeItem{ 85.0f, 85.0f };
		float window_visible_x2 = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			++numItems;
			ImGui::PushID(numItems);

			// apply SameLine if we are not printing the first item and if it fits into the remaining windows space
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + sizeItem.X; // Expected position if next button was on same line
			if (numItems != 1 && next_button_x2 < window_visible_x2)
			{
				ImGui::SameLine();
			}

			//ImVec2 pos = ImGui::GetCursorScreenPos();
			//ImVec2 marker_min = ImVec2(pos.x + sizeItem.X, pos.y);
			//ImVec2 marker_max = ImVec2(pos.x + sizeItem.X + 10, pos.y + ImGui::GetTextLineHeight());

			// get the name of the current item
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetsPath);
			std::string filename = relativePath.filename().string();
			
			// render directories
			if (directoryEntry.is_directory())
			{
				if(!RenderRenamingContentItem(path, numItems, filename, folderTexture, sizeItem))
				{
					if (RenderContentItem(numItems, filename, folderTexture, sizeItem))
					{
						m_CurrentDirectory /= path.filename();
						m_SelectedItem = -1;
					}
					RenderContentItemContextMenu(filename, path);
				}
			}
			else // render files
			{				
				if (m_ShowAllFiles)
				{
					if (!RenderRenamingContentItem(path, numItems, filename, fileTexture, sizeItem))
					{
						RenderContentItem(numItems, filename, fileTexture, sizeItem);
						RenderContentItemContextMenu(filename, path);
					}
				}
				else
				{
					if (relativePath.extension() == ASSET_EXTENSION)
					{
						if (!RenderRenamingContentItem(path, numItems, filename, fileTexture, sizeItem))
						{
							RenderContentItem(numItems, filename, fileTexture, sizeItem);
							RenderContentItemAssetContextMenu(filename, relativePath);
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

	bool ContentBrowserPanel::RenderContentItem(const int numItem, const std::string& text, const Texture* texture, Vector2f size, Color color)
	{
		ImVec2 buttonSize{ size.X, size.Y };
		ImVec4 tintColor{ color.R / 255.f, color.G / 255.f, color.B / 255.f, color.A / 255.f };

		return ImGui::SelectableItemWithImageAndText(
			m_SelectedItem,
			numItem,
			text,
			(ImTextureID)texture->GetNativeTexture(),
			buttonSize,
			tintColor,
			ImGuiSelectableFlags_AllowDoubleClick
		);
	}

	bool ContentBrowserPanel::RenderRenamingContentItem(const std::filesystem::path& path, const int numItem, const std::string& text, const Texture* texture, Vector2f size, Color color)
	{
		if (!IsRenaming(path)) return false;

		ImVec2 buttonSize{ size.X, size.Y };
		ImVec4 tintColor{ color.R / 255.f, color.G / 255.f, color.B / 255.f, color.A / 255.f };

		 bool renamed = ImGui::SelectableItemWithImageAndTextInput(
			m_SelectedItem,
			numItem,
			text,
			m_RenameBuffer,
			sizeof(m_RenameBuffer) / sizeof(char),
			(ImTextureID)texture->GetNativeTexture(),
			buttonSize,
			tintColor,
			ImGuiSelectableFlags_AllowDoubleClick
		);

		if (/*renamed || */ImGui::IsItemDeactivated())
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

	void ContentBrowserPanel::RenderRenameOption(const std::filesystem::path& path)
	{
		if (ImGui::MenuItem("Rename"))
		{
			m_PathToRename = path;
		}
	}

	void ContentBrowserPanel::RenderDeleteOption(const std::filesystem::path& path)
	{
		if (ImGui::MenuItem("Delete"))
		{
			std::filesystem::remove_all(path);
			m_SelectedItem = -1;
		}
	}

	void ContentBrowserPanel::RenderContentItemCommonOptions(const std::filesystem::path& path)
	{
		RenderRenameOption(path);
		RenderDeleteOption(path);
	}

	void ContentBrowserPanel::RenderContentItemContextMenu(const std::string& filename, const std::filesystem::path& path)
	{
		if (ImGui::BeginPopupContextItem(filename.c_str()))
		{
			RenderContentItemCommonOptions(path);
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::RenderContentItemAssetContextMenu(const std::string& filename, const std::filesystem::path& path)
	{
		const AssetMetadata& metadata = AssetManager::GetMetadataFromPath(s_AssetsPath / path);

		if (ImGui::BeginPopupContextItem(filename.c_str()))
		{
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

			RenderContentItemCommonOptions(path);
			ImGui::EndPopup();
		}
	}

	bool ContentBrowserPanel::IsRenaming(const std::filesystem::path& path)
	{
		return !m_PathToRename.string().empty() && m_PathToRename == path;
	}
}
