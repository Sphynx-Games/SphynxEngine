#pragma once

#include "Core/Core.h"
#include "Asset/Asset.h"
#include "Container/Array.h"


namespace Sphynx
{
	class SPHYNX_API Animation2D
	{
	public:
		Animation2D() : Sprites() {}
		Animation2D(Array<AssetHandle>& sprites) : Sprites(sprites) {}

	public:
		Array<AssetHandle> Sprites;
	};

	struct SPHYNX_API Animation2DAssetMetadata
	{
		Array<AssetHandle> Sprites;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Animation2D);

}

#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(Sphynx::Animation2D, SPHYNX_API)

SPX_REFLECT_PROPERTY(Sprites)

SPX_REFLECT_CLASS_END(Sphynx::Animation2D, SPHYNX_API)

SPX_REFLECT_STRUCT_BEGIN(Sphynx::Animation2DAssetMetadata, SPHYNX_API)

SPX_REFLECT_PROPERTY(Sprites)

SPX_REFLECT_STRUCT_END(Sphynx::Animation2DAssetMetadata, SPHYNX_API)