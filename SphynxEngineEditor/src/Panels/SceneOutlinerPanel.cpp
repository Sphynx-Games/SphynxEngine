#include <spxpch.h>
#include "SceneOutlinerPanel.h"

#include <imgui.h>

#include <Scene/Scene.h>
#include <Scene/Actor.h>
#include <Component/UUIDComponent.h>
#include <Component/NameComponent.h>

namespace Sphynx
{
	SceneOutlinerPanel::SceneOutlinerPanel(const Scene* scene) :
		m_Scene(scene)
	{
	}

	void SceneOutlinerPanel::SetContext(const Scene* scene)
	{
		m_Scene = scene;
	}

	const Actor& SceneOutlinerPanel::GetSelectedActor() const
	{
		return m_SelectedActor;
	}

	void SceneOutlinerPanel::RenderGUI()
	{
		if (ImGui::Begin("Outliner") && m_Scene != nullptr)
		{
			for (const Actor& actor : m_Scene->GetActors())
			{
				RenderActorGUI(actor);
			}
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
		ImGui::TreePop();

		// TODO: Render children recursivelly
	}

}
