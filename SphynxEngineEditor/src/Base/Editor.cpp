#include "spxpch.h"
#include "Editor.h"
#include "Toolbar.h"
#include <imgui.h>

namespace Sphynx
{
	Editor::Editor(const std::string& name) :
		m_Widgets(),
		m_Toolbar(),
		m_Name(name)
	{
	}

	Editor::~Editor()
	{
		for (Widget* widget : m_Widgets)
		{
			delete widget;
		}

		m_Widgets.clear();
		delete m_Toolbar;
	}

	void Editor::AddWidget(Widget* widget)
	{
		m_Widgets.push_back(widget);
	}

	void Editor::RemoveWidget(Widget* widget)
	{
		auto it = std::find(m_Widgets.begin(), m_Widgets.end(), widget);
		if (it == m_Widgets.end()) return;

		m_Widgets.erase(it);
	}

	void Editor::SetToolbar(Toolbar* toolbar)
	{
		m_Toolbar = toolbar;
	}

	void Editor::HandleEvent(Event& event)
	{
		for (Widget* widget : m_Widgets)
		{
			widget->HandleEvent(event);
		}
	}

	void Editor::RenderGUI()
	{
		ImGuiID id = ImGui::GetID(m_Name.c_str());
		if (ImGui::Begin(m_Name.c_str()))
		{
			m_Toolbar->RenderGUI();
			ImGui::DockSpace(id);
			for (Widget* widget : m_Widgets)
			{
				widget->RenderGUI();
			}
		}
		ImGui::End();
	}
}