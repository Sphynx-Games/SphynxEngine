#pragma once

#include "Base/Panel.h"
#include <imgui.h>


namespace Sphynx
{
	class DetailsPanel : public Panel
	{
	public:
		using Panel::Panel;

	protected:
		virtual void RenderGUI() override
		{
			ImGui::Begin("Details");
			ImGui::End();
		}
	};
}
