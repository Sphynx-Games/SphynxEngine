#include "spxpch.h"
#include "SaveButtonWidget.h"
#include "Base/Resources.h"
#include "ImGuiExtra.h"

#include <imgui.h>


namespace Sphynx
{
	void SaveButtonWidget::RenderGUI()
	{
		ImVec2 buttonSize{ Size.X, Size.Y };
		ImVec4 tintColor{ IconColor.R / 255.f, IconColor.G / 255.f, IconColor.B / 255.f, IconColor.A / 255.f };

		if (ImGui::ButtonWithImageAndText("Save", (ImTextureID)Resources::SaveTexture->GetNativeTexture(), buttonSize, 0.75f, tintColor))
		{
			OnClick.Execute();
		}
	}
}