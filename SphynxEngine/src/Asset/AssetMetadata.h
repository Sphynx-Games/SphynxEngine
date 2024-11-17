#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Traits/Utility.h"
#include "Asset.h"
#include "Container/Array.h"

#include <filesystem>


#define SPHYNX_ASSET_HEADER "SphynxAsset"

namespace Sphynx
{
	using AssetHandle = Traits::unique_type<UUID, struct __AssetHandle__>;

	struct AssetMetadataHeader
	{
		std::string SphynxAsset;
		AssetType Type;
	};

	struct AssetMetadata
	{
		AssetHandle Handle;
		AssetType Type;
		std::filesystem::path Path;
		Array<AssetHandle> Dependencies;

		static const AssetMetadata Invalid;
	};

	inline const AssetMetadata AssetMetadata::Invalid { AssetHandle::Invalid };
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::AssetMetadataHeader)
SPX_REFLECT_PROPERTY(SphynxAsset)
SPX_REFLECT_PROPERTY(Type)
SPX_REFLECT_STRUCT_END(Sphynx::AssetMetadataHeader)

SPX_REFLECT_STRUCT_BEGIN(Sphynx::AssetMetadata)
SPX_REFLECT_PROPERTY(Handle)
SPX_REFLECT_PROPERTY(Type)
SPX_REFLECT_PROPERTY(Path)
SPX_REFLECT_PROPERTY(Dependencies)
SPX_REFLECT_STRUCT_END(Sphynx::AssetMetadata)