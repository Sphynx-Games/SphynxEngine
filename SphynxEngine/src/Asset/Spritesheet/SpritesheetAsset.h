#pragma once

#include "Core/Core.h"
#include "Container/Map.h"
#include "Asset/Asset.h"
#include "Asset/AssetDependencySolver.h"
#include "Asset/SubAssetSolver.h"
#include "Asset/AssetManager.h"
#include "Asset/Sprite/SpriteAsset.h"


namespace Sphynx
{
	struct SPHYNX_API SpritesheetAssetMetadata
	{
		HashMap<AssetHandle, SpriteAssetMetadata> SpritesData;
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
			int i = 0;
			std::wstring name = metadata.Path.stem().wstring();
			for (Sprite* sprite : spritesheet.GetSprites())
			{
				std::wstring spriteName = name + L"_" + std::to_wstring(i);
				AssetManager::RegisterAsset(sprite, metadata.Path / spriteName, false);
				++i;
			}
		}
	};
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::SpritesheetAssetMetadata, SPHYNX_API)

	SPX_REFLECT_PROPERTY(SpritesData)

SPX_REFLECT_STRUCT_END(Sphynx::SpritesheetAssetMetadata, SPHYNX_API)
