#include "spxpch.h"
#include "Toolbar.h"
#include <imgui.h>

namespace Sphynx
{
	Toolbar::Toolbar(Widget* parent) :
		Widget(parent)
	{
	}

	Toolbar::~Toolbar()
	{
	}

	void Toolbar::RenderGUI()
	{
		const Array<Widget*>& children = GetChildren();
		for (int i = 0; i < children.Size(); ++i)
		{
			children[i]->RenderGUI();
			if (i != children.Size() - 1)
			{
				ImGui::SameLine();
			}
		}
	}
}