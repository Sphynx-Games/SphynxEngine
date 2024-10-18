#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"
#include "Asset/Asset.h"
#include "Asset/AssetDependencySolver.h"
#include "Asset/AssetManager.h"
#include "Renderer/Sprite.h"


namespace Sphynx
{
	struct SPHYNX_API SpriteAssetMetadata
	{
		Vector2i Position;
		Vector2i Size;
		Vector2f Pivot;
		uint32_t PixelsPerUnit;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Sprite);

	template<>
	struct SPHYNX_API AssetDependencySolver<Sprite>
	{
		void operator()(const Sprite& sprite, AssetMetadata& metadata)
		{
			AssetHandle handle = AssetManager::GetAssetHandleFromAddress(sprite.GetTexture());
			metadata.Dependencies.Add(handle);
		}
	};
}

#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::SpriteAssetMetadata)

	SPX_REFLECT_PROPERTY(Position)
	SPX_REFLECT_PROPERTY(Size)
	SPX_REFLECT_PROPERTY(Pivot)
	SPX_REFLECT_PROPERTY(PixelsPerUnit)

SPX_REFLECT_STRUCT_END(Sphynx::SpriteAssetMetadata)
