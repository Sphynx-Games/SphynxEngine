#include "spxpch.h"
#include "Toolbar.h"
#include <imgui.h>

namespace Sphynx
{
	Toolbar::Toolbar() :
		m_Widgets()
	{
	}

	Toolbar::~Toolbar()
	{
		for (Widget* widget : m_Widgets)
		{
			delete widget;
		}

		m_Widgets.clear();
	}

	void Toolbar::AddWidget(Widget* widget)
	{
		m_Widgets.push_back(widget);
	}

	void Toolbar::RemoveWidget(Widget* widget)
	{
		auto it = std::find(m_Widgets.begin(), m_Widgets.end(), widget);
		if (it == m_Widgets.end()) return;

		m_Widgets.erase(it);
	}

	void Toolbar::HandleEvent(Event& event)
	{
		for (Widget* widget : m_Widgets)
		{
			widget->HandleEvent(event);
		}
	}

	void Toolbar::RenderGUI()
	{
		for (int i = 0; i < m_Widgets.size(); ++i)
		{
			m_Widgets[i]->RenderGUI();
			if (i != m_Widgets.size() - 1)
			{
				ImGui::SameLine();
			}
		}
	}
}