#include <spxpch.h>
#include "SceneOutlinerPanel.h"
#include <imgui.h>
#include "Events/Event.h"
#include "Events/InputEvent.h"
#include "Input/Keycode.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Component/UUIDComponent.h"
#include "Component/NameComponent.h"

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
		if (ImGui::Begin(GetName()) && m_Scene != nullptr)
		{
			for (const Actor& actor : m_Scene->GetActors())
			{
				RenderActorGUI(actor);
			}
			for (auto it = m_ActorsToDeleted.rbegin(); it != m_ActorsToDeleted.rend(); it++)
			{
				DeleteActor(*it);
			}
			m_ActorsToDeleted.RemoveAll();
		}
		ImGui::End();
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
