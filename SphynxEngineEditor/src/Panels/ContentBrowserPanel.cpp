#include "spxpch.h"
#include "ContentBrowserPanel.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/TextureAsset.h"
#include "Asset/Sprite/SpriteAsset.h"
#include "Dialogs/FileDialog.h"
#include "Renderer/Texture.h"

#include <imgui.h>

#include <Windows.h>
#include <commdlg.h>


namespace Sphynx
{
	static const std::filesystem::path s_ResourcesPath = "SphynxEngineEditor\\Resources";

	static const std::filesystem::path s_AssetsPath = "Assets";

	std::filesystem::path MakeRelativePath(const std::filesystem::path& absolutePath) {
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
		m_ShowAllFiles(false)
	{
	}

	void ContentBrowserPanel::RenderGUI()
	{
		static Sphynx::Texture* folderTexture = Sphynx::TextureLoader::Load(s_ResourcesPath / "folder.png");
		static Sphynx::Texture* fileTexture = Sphynx::TextureLoader::Load(s_ResourcesPath / "file.png");


		ImGui::Begin("Content Browser");

		// back button
		ImGui::BeginDisabled(m_CurrentDirectory == s_AssetsPath);
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		float itemPadding = ImGui::GetStyle().ItemSpacing.x;
		float itemWidth1 = ImGui::CalcTextSize("Import").x + itemPadding * 2.f;
		float itemWidth2 = ImGui::CalcTextSize("Show all").x + itemPadding * 2.f;
		float widthNeeded = itemWidth1 + itemPadding + itemWidth2;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);

		// import button
		if (ImGui::Button("Import"))
		{
			std::filesystem::path path = FileDialog::Open();
			if (!path.empty())
			{
				AssetManager::Import(MakeRelativePath(path));
			}
		}

		ImGui::SameLine();

		// show all files checkbox
		ImGui::Checkbox("Show all", &m_ShowAllFiles);

		// content
		int numItems = 0;
		ImVec2 sizeItem{85.0f, 85.0f};
		float window_visible_x2 = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			++numItems;
			ImGui::PushID(numItems);

			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + sizeItem.x; // Expected position if next button was on same line
			if (numItems != 1 && next_button_x2 < window_visible_x2)
			{
				ImGui::SameLine();
			}

			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetsPath);
			std::string fileName = relativePath.filename().string();

			static float wrap_width = sizeItem.x;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
			ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());

			
			if (directoryEntry.is_directory()) // directories
			{
				ImGui::BeginGroup();

				if (ImGui::ImageButton(fileName.c_str(), (ImTextureID)folderTexture->GetNativeTexture(), sizeItem))
				{
					m_CurrentDirectory /= path.filename();
				}

				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
				ImGui::TextWrapped(fileName.c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();
			}
			else // files
			{				
				if (m_ShowAllFiles)
				{
					ImGui::BeginGroup();

					ImGui::ImageButton(fileName.c_str(), (ImTextureID)fileTexture->GetNativeTexture(), sizeItem);

					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
					ImGui::TextWrapped(fileName.c_str());
					ImGui::PopTextWrapPos();

					ImGui::EndGroup();
				}
				else
				{
					if (relativePath.extension() == ASSET_EXTENSION)
					{
						ImGui::BeginGroup();

						ImGui::ImageButton(fileName.c_str(), (ImTextureID)fileTexture->GetNativeTexture(), sizeItem);

						ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
						ImGui::TextWrapped(fileName.c_str());
						ImGui::PopTextWrapPos();

						ImGui::EndGroup();

						const AssetMetadata& metadata = AssetManager::GetMetadataFromPath(s_AssetsPath / relativePath);

						if (metadata.Type == TypeToAssetType<Texture>::Value && ImGui::BeginPopupContextItem(fileName.c_str()))
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
							ImGui::EndPopup();
						}
					}
				}
			}

			ImGui::PopID();
		}
		

		ImGui::End();
	}
}
