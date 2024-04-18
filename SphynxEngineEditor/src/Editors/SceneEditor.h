#pragma once

#include "Base/Editor.h"


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
		class SceneOutlinerPanel* m_SceneOutlinerPanel;
		class ContentBrowserPanel* m_ContentBrowserPanel;
		class ViewportPanel* m_ViewportPanel;
		class DetailsPanel* m_DetailsPanel;

		class Framebuffer* m_Framebuffer;
		class Scene* m_ActiveScene;
	};
}
