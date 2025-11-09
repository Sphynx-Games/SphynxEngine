#include "spxpch.h"
#include "AssetEditor.h"
#include "EditorLayer.h"
#include "Editors/PrefabEditor.h"
#include "Editors/PropertyEditor.h"

#include <Asset/AssetManager.h>
#include <Asset/Prefab/PrefabAsset.h>

#include <imgui.h>


namespace Sphynx
{
	AssetEditor::AssetEditor() :
		Editor("AssetEditor"),
		m_AssetEditors()
	{
	}

	void AssetEditor::RenderGUI()
	{
		// This will append menu bar items into the parents menu bar
		// But only if the window has not shown them already
		if (HasMenuBar())
		{
			if (ImGui::BeginMenuBar())
			{
				RenderMenuBar();
				ImGui::EndMenuBar();
			}
		}
	}

	void AssetEditor::RenderMenuBar()
	{
		/*if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}*/
	}

	void AssetEditor::OpenAssetEditor(const std::string& name, const AssetMetadata& metadata, EditorLayer* editorLayer)
	{
		if (m_AssetEditors.ContainsKey(metadata.Handle))
		{
			editorLayer->SetActiveEditor(m_AssetEditors[metadata.Handle].First);
			return;
		}

		m_AssetEditors.Add(metadata.Handle, { nullptr, "" });

		std::string& editorName = m_AssetEditors[metadata.Handle].Second;
		editorName = name;

		Editor*& assetEditor = m_AssetEditors[metadata.Handle].First;
		if (metadata.Type == TypeToAssetType<Prefab>::Value()) // TYPE_TO_ASSETTYPE(Prefab)
		{
			std::shared_ptr<Asset<Prefab>> prefab = AssetManager::GetAsset<Prefab>(metadata.Handle);
			assetEditor = new PrefabEditor(editorName.c_str(), prefab->Asset);
		}
		else
		{
			std::shared_ptr<IAsset> asset = AssetManager::GetAsset(metadata.Handle);
			assetEditor = new PropertyEditor(editorName.c_str(), static_cast<const Reflection::Class&>(*metadata.Type.Type), asset->GetRawAsset());
		}

		Widget::WidgetID id = std::hash<UUID>()(metadata.Handle);
		assetEditor->SetOverrideID(id);
		editorLayer->AddEditor(assetEditor);
		editorLayer->SetActiveEditor(assetEditor);
		assetEditor->OnClose.Bind([&]() {
			m_AssetEditors.Remove(metadata.Handle);
		});
	}
}