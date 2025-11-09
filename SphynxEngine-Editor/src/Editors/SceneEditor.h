#pragma once

#include "Base/Editor.h"
#include "EditorCameraController.h"
#include "ProjectInfo.h"
#include "Scene/EditorScene.h"
#include <Scene/Scene.h>
#include <Core/Delegate.h>
#include <Container/Map.h>
#include <Container/Pair.h>
#include <Asset/Asset.h>
#include <Utils/PlaybackState.h>


namespace Sphynx
{
	class EditorLayer;
	class SceneToolbar;
	class SceneOutlinerPanel;
	class ContentBrowserPanel;
	class ViewportPanel;
	class DetailsPanel;
	class Framebuffer;
	struct AssetMetadata;


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
		void OpenAssetEditor(const std::string& name, const AssetMetadata& metadata);

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
		DelegateHandle m_EditAssetHandle;

		EditorLayer* m_EditorLayer;
		SceneToolbar* m_SceneToolbar;
		SceneOutlinerPanel* m_SceneOutlinerPanel;
		ContentBrowserPanel* m_ContentBrowserPanel;
		ViewportPanel* m_ViewportPanel;
		DetailsPanel* m_DetailsPanel;

		Framebuffer* m_Framebuffer;
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
