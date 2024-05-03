#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include "Renderer/RendererTypes.h"

namespace Sphynx
{
	struct SPHYNX_API CapsuleCollider2DComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(CapsuleCollider2DComponent);

	public:
		CapsuleCollider2DComponent(Vector2f size = { 1.0f, 2.0f }, Vector2f pivot = { 0.5f, 0.5f }) :
			Size(size),
			Pivot(pivot)
		{}

	public:
		Vector2f Size;
		Vector2f Pivot;
	};
}
