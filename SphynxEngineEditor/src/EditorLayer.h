#pragma once

#include <Sphynx.h>


namespace Sphynx
{
	class Widget;
	class Event;

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void Attach() override;
		virtual void Detach() override;
		virtual void Update(float deltaTime) override;
		virtual void HandleEvent(Event& event) override;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void RenderGUI();

		bool IsBlockEventsEnabled() const;
		void SetBlockEventsEnabled(bool enabled);

		void AddWidget(Widget* widget);
		void RemoveWidget(Widget* widget);

	public:
		static EditorLayer* Create();

	protected:
		bool m_BlockEventsEnabled;
		std::vector<Widget*> m_Widgets;

	};
}
