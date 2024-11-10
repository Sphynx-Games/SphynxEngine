#pragma once

#include "Widget.h"
#include <vector>
#include <string>

namespace Sphynx
{
	class Panel;
	class Event;

	class Editor : public Widget
	{
	public:
		Editor(const std::string& name);
		virtual ~Editor();

	public:
		void AddPanel(Panel* panel);
		void RemovePanel(Panel* panel);

	protected:
		virtual void HandleEvent(Event& event) override;
		virtual void RenderGUI() override;

	protected:
		std::vector<Panel*> m_Panels;
		std::string m_Name;
	};
}
