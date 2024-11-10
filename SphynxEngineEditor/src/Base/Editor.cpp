#include "spxpch.h"
#include "Editor.h"

#include "Panel.h"
#include <imgui.h>

namespace Sphynx
{
	Editor::Editor(const std::string& name) :
		m_Panels(),
		m_Name(name)
	{
	}

	Editor::~Editor()
	{
		for (Panel* panel : m_Panels)
		{
			delete panel;
		}

		m_Panels.clear();
	}

	void Editor::AddPanel(Panel* panel)
	{
		m_Panels.push_back(panel);
	}

	void Editor::RemovePanel(Panel* panel)
	{
		auto it = std::find(m_Panels.begin(), m_Panels.end(), panel);
		if (it == m_Panels.end()) return;

		m_Panels.erase(it);
	}

	void Editor::HandleEvent(Event& event)
	{
		for (Panel* panel : m_Panels)
		{
			panel->HandleEvent(event);
		}
	}

	void Editor::RenderGUI()
	{
		ImGuiID id = ImGui::GetID(m_Name.c_str());
		if (ImGui::Begin(m_Name.c_str()))
		{
			ImGui::DockSpace(id);
			for (Panel* panel : m_Panels)
			{
				panel->RenderGUI();
			}
		}
		ImGui::End();
	}
}