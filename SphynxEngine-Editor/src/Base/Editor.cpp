#include "spxpch.h"
#include "Editor.h"
#include "Toolbar.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Sphynx
{
	Editor::Editor(const char* name, Widget* parent) :
		Widget(name, parent),
		m_Toolbar(),
		m_ID(0)
	{
	}

	Editor::~Editor()
	{
		delete m_Toolbar;
	}

	void Editor::SetToolbar(Toolbar* toolbar)
	{
		m_Toolbar = toolbar;
	}

	void Editor::SetOverrideID(WidgetID id)
	{
		m_ID = id;
	}

	void Editor::RenderGUI()
	{
		const ImGuiWindow* window = ImGui::FindWindowByName(GetName());
		const bool bIsWindowDocked = window != nullptr && window->DockNodeIsVisible;
		ImGuiWindowFlags flags = 0;
		flags |= !bIsWindowDocked * ImGuiWindowFlags_MenuBar;

		if (m_ID != 0) ImGui::PushOverrideID(m_ID);

		const bool visible = ImGui::Begin(GetName(), nullptr, flags);
		ImGui::PushID(GetName());
		if (visible)
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
				m_Toolbar->PreRenderGUI();
				m_Toolbar->RenderGUI();
				m_Toolbar->PostRenderGUI();
			}

			// Enable dockspace for the current editor
			ImGuiID id = ImGui::GetID(GetName());
			ImGui::DockSpace(id);

			// Render child widgets
			for (Widget* widget : GetChildren())
			{
				widget->PreRenderGUI();
				widget->RenderGUI();
				widget->PostRenderGUI();
			}
		}
		ImGui::PopID();
		ImGui::End();

		if (m_ID != 0) ImGui::PopID();

		// This will append menu bar items into the parents menu bar
		// But only if the window has not shown them already
		if (visible && bIsWindowDocked && HasMenuBar())
		{
			if (ImGui::BeginMenuBar())
			{
				RenderMenuBar();
				ImGui::EndMenuBar();
			}
		}
	}
}