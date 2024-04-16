#pragma once

#include "Base/Panel.h"
#include <imgui.h>


namespace Sphynx
{
	class ContentBrowserPanel : public Panel
	{
	public:
		using Panel::Panel;

	protected:
		virtual void RenderGUI() override
		{
			ImGui::Begin("Content Brower");
			ImGui::End();
		}
	};
}
