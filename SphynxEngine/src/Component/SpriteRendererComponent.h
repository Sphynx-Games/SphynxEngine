#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Color.h"


namespace Sphynx
{
	struct SPHYNX_API SpriteRendererComponent
	{
		COMPONENT_COMMON_BODY(SpriteRendererComponent);

	public:
		SpriteRendererComponent(Color tint) :
			Tint(tint) {}

	public:
		Color Tint;

	};
}
