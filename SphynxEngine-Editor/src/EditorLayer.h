#pragma once

#include <Sphynx.h>
#include <Operation/OperationManager.h>


namespace Sphynx
{
	class Editor;
	class AssetEditor;
	class Event;

	class EditorLayer final : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void Attach() override;
		virtual void Detach() override;
		virtual void Update(float deltaTime) override;
		virtual void HandleEvent(Event& event) override;

		void Begin();
		void End();
		void RenderGUI();

		bool IsBlockEventsEnabled() const;
		void SetBlockEventsEnabled(bool enabled);

		void AddEditor(Editor* editor);
		void RemoveEditor(Editor* editor);

		AssetEditor* GetAssetEditor() const;
		void SetActiveEditor(Editor* editor);

		OperationManager* GetCurrentOperationManager() const;

	protected:
		bool m_BlockEventsEnabled;

		OperationManager m_OperationManager;
		AssetEditor* m_AssetEditor;
		Array<Editor*> m_Editors;

		Editor* m_ActiveEditor;
		OperationManager* m_CurrentOperationManager;
	};
}
