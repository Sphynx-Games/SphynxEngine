#include "spxpch.h"
#include "SceneToolbar.h"
#include "Widgets/ButtonWidget.h"
#include "Base/Resources.h"
#include "Serialization/YAML/YAMLWriter.h"
#include "Serialization/SceneSerializer.h"

#include <imgui.h>


namespace Sphynx
{
	SceneToolbar::SceneToolbar() :
		m_SaveButton(new ButtonWidget("Save", Resources::SaveTexture)),
		m_PlayAndPauseButton(new ButtonWidget("Play", Resources::PlayTexture)),
		m_StopButton(new ButtonWidget("Stop", Resources::StopTexture))
		//m_SceneNameBuffer()
	{
		AddWidget(m_SaveButton);
		AddWidget(m_PlayAndPauseButton);
		AddWidget(m_StopButton);
	}

	void SceneToolbar::SetPlaybackState(PlaybackState state)
	{
		switch (state)
		{
		case PlaybackState::PLAYING:
			m_PlayAndPauseButton->Text = "Pause";
			m_PlayAndPauseButton->Image = Resources::PauseTexture;
			m_StopButton->IsEnabled = true;
			break;
		case PlaybackState::PAUSED:
			m_PlayAndPauseButton->Text = "Play";
			m_PlayAndPauseButton->Image = Resources::PlayTexture;
			m_StopButton->IsEnabled = true;
			break;
		case PlaybackState::STOPPED:
			m_PlayAndPauseButton->Text = "Play";
			m_PlayAndPauseButton->Image = Resources::PlayTexture;
			m_StopButton->IsEnabled = false;
			break;
		default:
			break;
		}

	}

	/*	if (ImGui::BeginPopup("SceneSaved"))
	{
		ImGui::Text("Scene saved...");
		ImGui::EndPopup();
	}*/

	//strncpy(m_SceneNameBuffer, m_ActiveScene->GetName().c_str(), m_ActiveScene->GetName().size());
	//bool renamed = ImGui::InputText("##SceneName", m_SceneNameBuffer, sizeof(m_SceneNameBuffer) / sizeof(char));
}