#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Color.h"
#include "Math/Vector.h"


namespace Sphynx
{
	struct SPHYNX_API LineRendererComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(LineRendererComponent);

	public:
		LineRendererComponent(Vector2f begin = { -0.5f, 0.0f }, Vector2f end = { 0.5f, 0.0f }, float lineWidth = 1.0f, Color tint = Color::White) :
			Point1(begin), Point2(end), LineWidth(lineWidth), Color(tint) {}

	public:
		Vector2f Point1;
		Vector2f Point2;
		float LineWidth;
		Color Color;

	};
}

#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::LineRendererComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(Point1)
SPX_REFLECT_PROPERTY(Point2)
SPX_REFLECT_PROPERTY(LineWidth)
SPX_REFLECT_PROPERTY(Color)

SPX_REFLECT_STRUCT_END(Sphynx::LineRendererComponent, SPHYNX_API)