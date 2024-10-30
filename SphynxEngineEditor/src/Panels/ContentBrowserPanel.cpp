#include "spxpch.h"
#include "ContentBrowserPanel.h"
#include "Asset/AssetManager.h"
#include "Dialogs/FileDialog.h"
#include "Renderer/Texture.h"

#include <imgui.h>

#include <Windows.h>
#include <commdlg.h>


namespace Sphynx
{
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


	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetsPath),
		m_ShowAllFiles(false)
	{
	}

	void ContentBrowserPanel::RenderGUI()
	{
		static auto data{ 0xFFFFFFFF };
		static Sphynx::Texture* whiteTexture = Sphynx::Texture::Create(&data, Vector2i(1,1));


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
		if (ImGui::Checkbox("Show all", &m_ShowAllFiles))
		{
		}

		// content
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetsPath);
			std::string fileName = relativePath.filename().string();
			if (directoryEntry.is_directory()) // directories
			{
				if (ImGui::Button(fileName.c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else // files
			{
				if (m_ShowAllFiles)
				{
					ImGui::ImageButton(fileName.c_str(), (ImTextureID)whiteTexture->GetNativeTexture(), ImVec2(32, 32));
					ImGui::TextWrapped(fileName.c_str());
				}
				else
				{
					if (relativePath.extension() == ASSET_EXTENSION)
					{
						ImGui::ImageButton(fileName.c_str(), (ImTextureID)whiteTexture->GetNativeTexture(), ImVec2(32, 32));
						ImGui::TextWrapped(fileName.c_str());
					}
				}
			}
		}
		

		ImGui::End();
	}
}
