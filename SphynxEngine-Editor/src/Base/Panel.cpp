#include "Panel.h"

#include "imgui.h"


namespace Sphynx
{
	void Panel::PreRenderGUI()
	{
		Widget::PreRenderGUI();
		std::string labelAndID{ GetName() };
		labelAndID += ("###" + std::to_string(GetID()));
		m_CanRender = ImGui::Begin(labelAndID.c_str());
	}

	void Panel::RenderGUI()
	{
		if (m_CanRender)
		{
			Widget::RenderGUI();
		}
	}

	void Panel::PostRenderGUI()
	{
		m_Position = { (uint32_t)ImGui::GetWindowPos().x, (uint32_t)ImGui::GetWindowPos().y };
		m_Size = { (uint32_t)ImGui::GetWindowWidth(), (uint32_t)ImGui::GetWindowHeight() };
		ImGui::End();
	}
}
