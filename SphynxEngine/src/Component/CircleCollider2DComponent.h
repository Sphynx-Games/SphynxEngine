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
		CircleCollider2DComponent(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }) :
			Radius(radius),
			Offset(offset)
		{}

	public:
		float Radius;
		Vector2f Offset;
	};
}
