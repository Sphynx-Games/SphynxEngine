#include "ButtonWidget.h"
#include "spxpch.h"
#include "ButtonWidget.h"
#include "Renderer/Texture.h"
#include "ImGuiExtra.h"

#include <imgui.h>


namespace Sphynx
{
	ButtonWidget::ButtonWidget(const std::string& text, Texture* image, Vector2f size, Color color, bool isActive, bool isEnabled) :
		OnClick(),
		Text(text),
		Image(image),
		Size(size),
		ImageColor(color),
		IsActive(isActive),
		IsEnabled(isEnabled)
	{
	}

	void ButtonWidget::RenderGUI()
	{
		if (!IsActive) return;

		if (!IsEnabled)
		{
			ImGui::BeginDisabled();
			RenderButton();
			ImGui::EndDisabled();
		}
		else
		{
			RenderButton();
		}
	}

	void ButtonWidget::RenderButton()
	{
		ImVec2 buttonSize{ Size.X, Size.Y };
		ImVec4 tintColor{ ImageColor.R / 255.f, ImageColor.G / 255.f, ImageColor.B / 255.f, ImageColor.A / 255.f };

		if (ImGui::ButtonWithImageAndText(Text, (ImTextureID)Image->GetNativeTexture(), buttonSize, 0.75f, tintColor))
		{
			OnClick.Broadcast();
		}
	}
}