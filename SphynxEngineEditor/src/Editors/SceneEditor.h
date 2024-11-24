#pragma once

#include "Base/Editor.h"
#include "Utils/PlaybackState.h"
#include "Scene/Scene.h"


namespace Sphynx
{
	class SceneEditor : public Editor
	{
	public:
		SceneEditor();
		virtual ~SceneEditor();

	protected:
		virtual void RenderGUI() override;

	private:
		void PlayScene();
		void PauseScene();
		void ResumeScene();
		void StopScene();

	private:
		class SceneToolbar* m_SceneToolbar;
		class SceneOutlinerPanel* m_SceneOutlinerPanel;
		class ContentBrowserPanel* m_ContentBrowserPanel;
		class ViewportPanel* m_ViewportPanel;
		class DetailsPanel* m_DetailsPanel;

		class Framebuffer* m_Framebuffer;
		Scene m_SceneToEdit;
		Scene m_SceneToPlay;
		Scene* m_ActiveScene;
		PlaybackState m_SceneState;
		char m_SceneNameBuffer[1024];
	};
}
