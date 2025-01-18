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
		Vector2i Position{ 0, 0 };
		Vector2i Size{ 1, 1 };
		Vector2f Pivot{ 0.5f, 0.5f };
		uint32_t PixelsPerUnit{ 100 };
	};

	GENERATE_ASSETTYPE_CONVERSOR(Sprite);

	template<>
	struct SPHYNX_API AssetDependencySolver<Sprite>
	{
		void operator()(const Sprite& sprite, AssetMetadata& metadata)
		{
			AssetHandle handle;
			if (metadata.Path.parent_path().extension() == ASSET_EXTENSION)
			{
				std::filesystem::path spritesheetPath = metadata.Path.parent_path();
				handle = AssetManager::GetAssetHandleFromPath(spritesheetPath);
			}
			else
			{
				handle = AssetManager::GetAssetHandleFromAddress(sprite.GetTexture());
			}
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
