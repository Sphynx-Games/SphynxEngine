#pragma once

#include "Base/Widget.h"
#include "Core/Delegate.h"
#include "Math/Vector.h"
#include "Math/Color.h"


namespace Sphynx
{
	class SaveButtonWidget : public Widget
	{
	public:
		SaveButtonWidget() = default;

	public:
		virtual void RenderGUI() override;

	public:
		Delegate<void()> OnClick;

		Vector2f Size{ 55.0f, 55.0f };
		Color IconColor = Color::White;
	};
}
