#pragma once

#include "Base/Editor.h"
#include "Utils/PlaybackState.h"
#include "Scene/Scene.h"
#include "EditorCameraController.h"
#include "ProjectInfo.h"
#include "Core/Delegate.h"
#include "Scene/EditorScene.h"


namespace Sphynx
{
	class EditorLayer;

	class SceneEditor : public Editor
	{
	public:
		SceneEditor(EditorLayer* editorLayer);
		virtual ~SceneEditor();

	protected:
		virtual void RenderGUI() override;
		virtual void PostRenderGUI() override;
		virtual void HandleEvent(Event& event) override;

		virtual bool HasMenuBar() const override { return true; }
		virtual void RenderMenuBar() override;

	private:
		void ManageProjectOpened(const ProjectInfo& projectInfo);
		void OpenPrefabEditor(Prefab* prefab);

		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveAsScene(const std::filesystem::path& path);

		void PlayScene();
		void PauseScene();
		void ResumeScene();
		void StopScene();
		void ActivateEject();
		void DeactivateEject();

		bool ShouldUseEditorCamera();

	private:
		DelegateHandle m_OpenedProjectHandle;
		DelegateHandle m_EditPrefabHandle;

		EditorLayer* m_EditorLayer;
		class SceneToolbar* m_SceneToolbar;
		class SceneOutlinerPanel* m_SceneOutlinerPanel;
		class ContentBrowserPanel* m_ContentBrowserPanel;
		class ViewportPanel* m_ViewportPanel;
		class DetailsPanel* m_DetailsPanel;

		class Framebuffer* m_Framebuffer;
		EditorCameraController m_CameraController;

		std::filesystem::path m_LastOpenedScenePath;
		EditorScene m_SceneToEdit;
		Scene m_SceneToPlay;
		Scene* m_ActiveScene;
		PlaybackState m_SceneState;
		bool m_Ejected;
		char m_SceneNameBuffer[1024];
	};
}
