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
		UV0(0.0f, 0.0f),
		UV1(1.0f, 1.0f),
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
		ImVec2 uv0 = { UV0.X, UV0.Y };
		ImVec2 uv1 = { UV1.X, UV1.Y };

		if (Image != nullptr)
		{
			if (ImGui::ButtonWithImageAndText(Text, (ImTextureID)Image->GetNativeTexture(), buttonSize, 0.75f, uv0, uv1, tintColor))
			{
				OnClick.Broadcast();
			}
		}
		else 
		{
			if (ImGui::Button(Text.c_str(), buttonSize))
			{
				OnClick.Broadcast();
			}
		}
	}
}