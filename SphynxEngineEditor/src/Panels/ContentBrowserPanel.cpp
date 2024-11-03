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
		m_SelectedItem(-1)
	{
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
			std::string fileName = relativePath.filename().string();
			
			// render directories
			if (directoryEntry.is_directory())
			{
				if (RenderSelectableItemWithImageAndText(numItems, fileName, folderTexture, sizeItem))
				{
					m_CurrentDirectory /= path.filename();
					m_SelectedItem = -1;
				}
				if (ImGui::BeginPopupContextItem(fileName.c_str()))
				{
					RenderDeleteButton(path);
					ImGui::EndPopup();
				}
			}
			else // render files
			{				
				if (m_ShowAllFiles)
				{
					RenderSelectableItemWithImageAndText(numItems, fileName, fileTexture, sizeItem);
					if (ImGui::BeginPopupContextItem(fileName.c_str()))
					{
						RenderDeleteButton(path);
						ImGui::EndPopup();
					}
				}
				else
				{
					if (relativePath.extension() == ASSET_EXTENSION)
					{
						RenderSelectableItemWithImageAndText(numItems, fileName, fileTexture, sizeItem);

						// context menu "Create sprite" for .spxasset of Texture type
						const AssetMetadata& metadata = AssetManager::GetMetadataFromPath(s_AssetsPath / relativePath);

						if (ImGui::BeginPopupContextItem(fileName.c_str()))
						{
							if(metadata.Type == TypeToAssetType<Texture>::Value)
							{
								if (ImGui::Button("Create sprite"))
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

							RenderDeleteButton(path);
							ImGui::EndPopup();
						}
					}
				}
			}

			ImGui::PopID();
		}
		

		ImGui::End();
	}

	bool ContentBrowserPanel::RenderSelectableItemWithImageAndText(const int numItem, const std::string& text, const Texture* texture, Vector2f size, Color color)
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

	void ContentBrowserPanel::RenderDeleteButton(const std::filesystem::path& path)
	{
		if (ImGui::Button("Delete"))
		{
			std::filesystem::remove_all(path);
			m_SelectedItem = -1;
		}
	}
}
