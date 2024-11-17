#include "spxpch.h"
#include "SaveButtonWidget.h"
#include "Renderer/Texture.h"
#include "ImGuiExtra.h"

#include <imgui.h>


namespace Sphynx
{
	void SaveButtonWidget::RenderGUI()
	{
		static const std::filesystem::path s_ResourcesPath = "SphynxEngineEditor\\Resources";
		static Texture* texture = TextureLoader::Load(s_ResourcesPath / "save.png");

		ImVec2 buttonSize{ Size.X, Size.Y };
		ImVec4 tintColor{ IconColor.R / 255.f, IconColor.G / 255.f, IconColor.B / 255.f, IconColor.A / 255.f };

		if (ImGui::ButtonWithImageAndText("Save", (ImTextureID)texture->GetNativeTexture(), buttonSize, 0.75f, tintColor))
		{
			OnClick.Execute();
		}
	}
}