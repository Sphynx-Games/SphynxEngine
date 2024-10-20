#pragma once

#include "Core/Core.h"
//#include "Math/Vector.h"
#include "Container/Array.h"
#include "Asset/Asset.h"
#include "Asset/AssetDependencySolver.h"
#include "Asset/SubAssetSolver.h"
#include "Asset/AssetManager.h"
#include "Asset/Sprite/SpriteAsset.h"
//#include "Renderer/Sprite.h"


namespace Sphynx
{
	struct SPHYNX_API SpritesheetAssetMetadata
	{
		Array<AssetHandle> SpritesHandles;
		Array<SpriteAssetMetadata> SpritesMetadatas;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Spritesheet);

	template<>
	struct SPHYNX_API AssetDependencySolver<Spritesheet>
	{
		void operator()(const Spritesheet& spritesheet, AssetMetadata& metadata)
		{
			AssetHandle handle = AssetManager::GetAssetHandleFromAddress(spritesheet.GetTexture());
			metadata.Dependencies.Add(handle);
		}
	};

	template<>
	struct SPHYNX_API SubAssetSolver<Spritesheet>
	{
		void operator()(const Spritesheet& spritesheet, AssetMetadata& metadata)
		{
			for (Sprite* sprite : spritesheet.GetSprites())
			{
				AssetManager::RegisterAsset(sprite, metadata.Path, false);
			}
		}
	};
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::SpritesheetAssetMetadata)

	SPX_REFLECT_PROPERTY(SpritesHandles)
	SPX_REFLECT_PROPERTY(SpritesMetadatas)

SPX_REFLECT_STRUCT_END(Sphynx::SpritesheetAssetMetadata)
