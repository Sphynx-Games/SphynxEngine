#pragma once

#include <Sphynx.h>


namespace Sphynx
{
	class Editor;
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

	protected:
		bool m_BlockEventsEnabled;
		std::vector<Editor*> m_Editors;

		Editor* m_ActiveEditor;
	};
}
