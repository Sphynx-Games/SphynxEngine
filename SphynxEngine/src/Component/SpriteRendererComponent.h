#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Color.h"


namespace Sphynx
{
	class Sprite;

	struct SPHYNX_API SpriteRendererComponent
	{
		COMPONENT_COMMON_BODY(SpriteRendererComponent);

	public:
		SpriteRendererComponent(Sprite* sprite, Color tint = Color::White) :
			Sprite(sprite), Tint(tint) {}

	public:
		Sprite* Sprite;
		Color Tint;

	};
}
