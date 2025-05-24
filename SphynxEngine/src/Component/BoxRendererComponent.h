#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Renderer/RendererTypes.h"
#include "Math/Vector.h"
#include "Math/Color.h"


namespace Sphynx
{
	struct SPHYNX_API BoxRendererComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(BoxRendererComponent);

	public:
		BoxRendererComponent(Vector2f size = { 1.0, 1.0 }, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White) :
			DrawMode(DrawMode::WIREFRAME), Size(size), Pivot(pivot), Color(color) {}

		BoxRendererComponent(DrawMode drawMode, Vector2f size = { 1.0, 1.0 }, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White) :
			DrawMode(drawMode), Size(size), Pivot(pivot), Color(color) {}

	public:
		DrawMode DrawMode;
		Vector2f Size;
		Vector2f Pivot;
		Color Color;

	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::BoxRendererComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(DrawMode)
SPX_REFLECT_PROPERTY(Size)
SPX_REFLECT_PROPERTY(Pivot)
SPX_REFLECT_PROPERTY(Color)

SPX_REFLECT_STRUCT_END(Sphynx::BoxRendererComponent, SPHYNX_API)