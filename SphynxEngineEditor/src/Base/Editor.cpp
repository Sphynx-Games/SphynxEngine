#include "spxpch.h"
#include "Editor.h"
#include "Toolbar.h"
#include <imgui.h>
#include <imgui_internal.h>

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

	const std::string& Editor::GetName() const
	{
		return m_Name;
	}

	void Editor::HandleEvent(Event& event)
	{
		for (Widget* widget : m_Widgets)
		{
			widget->HandleEvent(event);
		}
	}

	void Editor::PreRenderGUI()
	{
		for (Widget* widget : m_Widgets)
		{
			widget->PreRenderGUI();
		}
	}

	void Editor::RenderGUI()
	{
		const ImGuiWindow* window = ImGui::FindWindowByName(m_Name.c_str());
		const bool bIsWindowDocked = window != nullptr && window->DockNodeIsVisible;
		ImGuiWindowFlags flags = 0;
		flags |= !bIsWindowDocked * ImGuiWindowFlags_MenuBar;

		if (ImGui::Begin(m_Name.c_str(), nullptr, flags))
		{
			// Render Menu Bar
			if (ImGui::BeginMenuBar())
			{
				RenderMenuBar();
				ImGui::EndMenuBar();
			}

			// Render Toolbar (if valid)
			if (m_Toolbar != nullptr)
			{
				m_Toolbar->RenderGUI();
			}

			// Enable dockspace for the current editor
			ImGuiID id = ImGui::GetID("Editor Dockspace");
			ImGui::DockSpace(id);

			// Render child widgets
			for (Widget* widget : m_Widgets)
			{
				widget->RenderGUI();
			}
		}
		ImGui::End();

		// This will append menu bar items into the parents menu bar
		// But only if the window has not shown them already
		if (bIsWindowDocked && HasMenuBar())
		{
			if (ImGui::BeginMenuBar())
			{
				RenderMenuBar();
				ImGui::EndMenuBar();
			}
		}
	}

	void Editor::PostRenderGUI()
	{
		for (Widget* widget : m_Widgets)
		{
			widget->PostRenderGUI();
		}
	}
}