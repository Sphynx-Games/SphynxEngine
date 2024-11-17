#include "spxpch.h"
#include "SceneOptionsPanel.h"
#include "Widgets/SaveButtonWidget.h"
#include "Scene/Scene.h"
#include "Serialization/FileWriter.h"
#include "Serialization/SceneSerializer.h"

#include <imgui.h>


namespace Sphynx
{
	SceneOptionsPanel::SceneOptionsPanel(const Scene* scene) :
		m_Scene(scene),
		m_SaveButton(new SaveButtonWidget())
		//m_SceneNameBuffer()
	{
	}

	void SceneOptionsPanel::SetContext(const Scene* scene)
	{
		m_Scene = scene;

		m_SaveButton->OnClick.Bind([&]() {

			FileWriter writer = FileWriter("Assets\\Scenes\\" + m_Scene->GetName() + ".txt");
			SceneSerializer sceneSerializer = SceneSerializer(*m_Scene, writer);
			sceneSerializer.Serialize();

			ImGui::OpenPopup("SceneSaved");

		});
	}

	void SceneOptionsPanel::RenderGUI() 
	{
		ImGui::Begin("Options");

		m_SaveButton->RenderGUI();

		ImGui::SameLine();

		if (ImGui::BeginPopup("SceneSaved"))
		{
			ImGui::Text("Scene saved...");
			ImGui::EndPopup();
		}

		//strncpy(m_SceneNameBuffer, m_ActiveScene->GetName().c_str(), m_ActiveScene->GetName().size());
		//bool renamed = ImGui::InputText("##SceneName", m_SceneNameBuffer, sizeof(m_SceneNameBuffer) / sizeof(char));

		ImGui::End();
	}
}