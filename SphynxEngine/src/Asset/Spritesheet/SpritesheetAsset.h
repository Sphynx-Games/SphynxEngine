#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"
#include "Container/Array.h"
#include "Asset/Asset.h"
#include "Renderer/Sprite.h"


namespace Sphynx
{
	struct SPHYNX_API SpritesheetAssetMetadata
	{
		Array<AssetHandle> SpritesHandles;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Spritesheet);
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::SpritesheetAssetMetadata)

//SPX_REFLECT_PROPERTY(SpritesHandles)

SPX_REFLECT_STRUCT_END(Sphynx::SpritesheetAssetMetadata)
