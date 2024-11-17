#pragma once

#include "Base/Widget.h"
#include <vector>
#include <string>


namespace Sphynx
{
	class Event;

	class Editor : public Widget
	{
	public:
		Editor(const std::string& name);
		virtual ~Editor();

	public:
		void AddWidget(Widget* widget);
		void RemoveWidget(Widget* widget);

	protected:
		virtual void HandleEvent(Event& event) override;
		virtual void RenderGUI() override;

	protected:
		std::vector<Widget*> m_Widgets;
		std::string m_Name;
	};
}
