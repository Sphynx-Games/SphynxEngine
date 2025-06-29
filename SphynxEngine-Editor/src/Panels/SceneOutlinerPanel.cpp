#include <spxpch.h>
#include "SceneOutlinerPanel.h"
#include <Events/Event.h>
#include <Events/InputEvent.h>
#include <Input/Keycode.h>
#include <Scene/Scene.h>
#include <Component/UUIDComponent.h>
#include <Component/NameComponent.h>
#include <Component/TransformComponent.h>
#include <Serialization/Prefab/PrefabDeserializer.h>
#include <Asset/AssetManager.h>
#include <Asset/Prefab/PrefabAsset.h>

#include "Scene/EditorScene.h"

#include <imgui.h>


namespace Sphynx
{
	SceneOutlinerPanel::SceneOutlinerPanel(Scene* scene, Widget* parent) :
		Panel("Outliner", parent),
		m_Scene(scene),
		m_SelectedActor(),
		m_ActorsToDeleted()
	{
	}

	void SceneOutlinerPanel::SetContext(Scene* scene)
	{
		m_Scene = scene;
	}

	const Actor& SceneOutlinerPanel::GetSelectedActor() const
	{
		return m_SelectedActor;
	}

	void SceneOutlinerPanel::RenderGUI()
	{
		if (!m_CanRender) return;
		if (m_Scene == nullptr) return;

		// ACTORS
		for (const Actor& actor : m_Scene->GetActors())
		{
			RenderActorGUI(actor);
		}
		for (auto it = m_ActorsToDeleted.rbegin(); it != m_ActorsToDeleted.rend(); it++)
		{
			DeleteActor(*it);
		}
		m_ActorsToDeleted.RemoveAll();

		// PANEL CONTEXT MENU
		if (ImGui::BeginPopupContextWindow("SceneOutlinerPanel_ContextMenu", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
		{
			if (ImGui::MenuItem("New actor"))
			{
				Actor& actor = m_Scene->CreateActor();
				actor.AddComponent<NameComponent>("New actor");
				actor.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
			}

			if (!m_Scene->HasBegunPlay() && ImGui::BeginMenu("New prefab actor"))
			{
				Array<AssetMetadata> metadatas = AssetManager::GetAssetMetadatasByAssetType(TYPE_TO_ASSETTYPE(Prefab));
				for (const auto& m : metadatas)
				{
					auto filename = m.Path.filename();
					filename.replace_extension("");
					auto name = filename.string();
					if (ImGui::MenuItem(name.c_str()))
					{
						std::shared_ptr<Asset<Prefab>> prefabAsset = AssetManager::GetAsset<Prefab>(m.Handle);
						static_cast<EditorScene*>(m_Scene)->CreatePrefabActor(prefabAsset->Asset);
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}

	void SceneOutlinerPanel::RenderActorGUI(const Actor& actor)
	{
		bool hasName = actor.HasComponent<NameComponent>();
		std::string label = hasName ? actor.GetComponent<NameComponent>().Name : "Unnamed";

		ImGuiTreeNodeFlags flags = ((m_SelectedActor == actor) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Leaf;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)&actor.GetComponent<UUIDComponent>().UUID, flags, label.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedActor = actor;
		}
		RenderActorGUI_ContextMenu(actor);
		ImGui::TreePop();

		// TODO: Render children recursivelly
	}

	void SceneOutlinerPanel::RenderActorGUI_ContextMenu(const Actor& actor)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
			{
				m_ActorsToDeleted.Add(actor);
			}
			ImGui::EndPopup();
		}
	}

	void SceneOutlinerPanel::DeleteActor(const Actor& actor)
	{
		if (m_SelectedActor == actor)
		{
			m_SelectedActor = Actor();
		}
		m_Scene->DestroyActor(actor);
	}

}
