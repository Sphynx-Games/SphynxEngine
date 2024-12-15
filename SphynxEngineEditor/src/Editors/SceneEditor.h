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

		virtual bool HasMenuBar() const override { return true; }
		virtual void RenderMenuBar() override;

	private:
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveAsScene(const std::filesystem::path& path);

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
		std::filesystem::path m_LastOpenedScenePath;
		Scene m_SceneToEdit;
		Scene m_SceneToPlay;
		Scene* m_ActiveScene;
		PlaybackState m_SceneState;
		char m_SceneNameBuffer[1024];
	};
}
