#pragma once

#include "Base/Widget.h"
#include "Core/Delegate.h"
#include "Math/Vector.h"
#include "Math/Color.h"

#include <string>


namespace Sphynx
{
	class ButtonWidget : public Widget
	{
	public:
		ButtonWidget(const std::string& text, class Texture* image, Vector2f size = { 55.0f, 55.0f }, Color color = Color::White, bool isActive = true);

	public:
		virtual void RenderGUI() override;

	public:
		Delegate<void()> OnClick;

		std::string Text;
		Texture* Image;
		Vector2f Size;
		Color ImageColor;

		bool IsActive;
	};
}
