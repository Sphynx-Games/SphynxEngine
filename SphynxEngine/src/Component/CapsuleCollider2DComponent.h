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
		CapsuleCollider2DComponent(Vector2f size = { 1.0f, 2.0f }, Vector2f offset = { 0.0f, 0.0f }) :
			Size(size),
			Offset(offset)
		{}

	public:
		Vector2f Size;
		Vector2f Offset;
	};
}
