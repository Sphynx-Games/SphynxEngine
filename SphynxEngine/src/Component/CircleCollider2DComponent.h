#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include "Renderer/RendererTypes.h"

namespace Sphynx
{
	struct SPHYNX_API CircleCollider2DComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(CircleCollider2DComponent);

	public:
		CircleCollider2DComponent(float radius = 1.0f, Vector2f pivot = { 0.5f, 0.5f }) :
			Radius(radius),
			Pivot(pivot)
		{}

	public:
		float Radius;
		Vector2f Pivot;
	};
}
