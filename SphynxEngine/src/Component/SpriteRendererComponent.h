#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Color.h"
#include "Asset/Asset.h"


namespace Sphynx
{
	class Sprite;

	struct SPHYNX_API SpriteRendererComponent
	{
		COMPONENT_COMMON_BODY(SpriteRendererComponent);

	public:
		SpriteRendererComponent(AssetHandle sprite, Color tint = Color::White) :
			Sprite(sprite), Tint(tint) {}

	public:
		AssetHandle Sprite;
		Color Tint;

	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::SpriteRendererComponent)

SPX_REFLECT_PROPERTY(Sprite)
SPX_REFLECT_PROPERTY(Tint)

SPX_REFLECT_STRUCT_END(Sphynx::SpriteRendererComponent)