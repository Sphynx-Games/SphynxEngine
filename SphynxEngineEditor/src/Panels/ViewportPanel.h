#pragma once

#include "Base/Panel.h"
#include <imgui.h>


class ViewportPanel : public Panel
{
public:
	using Panel::Panel;

protected:
	virtual void RenderGUI() override
	{
		ImGui::Begin("Viewport");
		ImGui::End();
	}
};
