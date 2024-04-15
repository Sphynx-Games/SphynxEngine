#pragma once

#include "Base/Panel.h"
#include <imgui.h>
#include "SDL3/SDL.h"
#include <EditorLayer.h>


class ViewportPanel : public Panel
{
public:
	using Panel::Panel;

protected:
	virtual void RenderGUI() override
	{
		ImGui::Begin("Viewport");
		ImVec2 size = ImGui::GetWindowSize();
		EditorLayer::s_Framebuffer->GetColorAttachment(0);
		ImGui::Image(EditorLayer::s_Framebuffer->GetColorAttachment(0), size);

		ImGui::End();
	}
};
