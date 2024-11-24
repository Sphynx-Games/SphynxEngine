#pragma once

#include "Base/Editor.h"
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
		class SceneOptionsPanel* m_SceneOptionsPanel;
		class SceneOutlinerPanel* m_SceneOutlinerPanel;
		class ContentBrowserPanel* m_ContentBrowserPanel;
		class ViewportPanel* m_ViewportPanel;
		class DetailsPanel* m_DetailsPanel;

		class Framebuffer* m_Framebuffer;
		Scene m_SceneToEdit;
		Scene m_SceneToPlay;
		Scene* m_ActiveScene;
		bool m_IsSceneActive;
		char m_SceneNameBuffer[1024];
	};
}
