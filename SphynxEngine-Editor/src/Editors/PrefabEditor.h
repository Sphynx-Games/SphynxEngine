#pragma once

#include "Base/Editor.h"
#include <Scene/Scene.h>
#include "EditorCameraController.h"


namespace Sphynx
{
	class Prefab;
	class Event;

	class PrefabEditor : public Editor
	{
	public:
		PrefabEditor(Prefab* prefab);
		virtual ~PrefabEditor();

	protected:
		virtual void RenderGUI() override;
		virtual void HandleEvent(Event& event) override;

		// Menu bar
		virtual bool HasMenuBar() const override { return true; }
		virtual void RenderMenuBar() override;

	private:
		void SavePrefab();

	private:
		class SceneOutlinerPanel* m_SceneOutlinerPanel;
		class ViewportPanel* m_ViewportPanel;
		class DetailsPanel* m_DetailsPanel;
		class Framebuffer* m_Framebuffer;

		Prefab* m_Prefab;
		Scene m_Scene;

		EditorCameraController m_CameraController;
	};
}