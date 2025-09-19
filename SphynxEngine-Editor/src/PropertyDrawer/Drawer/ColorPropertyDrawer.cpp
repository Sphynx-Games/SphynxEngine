#include "ColorPropertyDrawer.h"

#include <imgui.h>


namespace Sphynx
{
	void ColorPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		Color* pColor = static_cast<Color*>(data);
		float color[4] =
		{
			pColor->R / 255.0f,
			pColor->G / 255.0f,
			pColor->B / 255.0f,
			pColor->A / 255.0f
		};
		if (ImGui::ColorEdit4("##", color))
		{
			pColor->R = (uint8_t)(color[0] * 255.0f);
			pColor->G = (uint8_t)(color[1] * 255.0f);
			pColor->B = (uint8_t)(color[2] * 255.0f);
			pColor->A = (uint8_t)(color[3] * 255.0f);
		}
	}
}
