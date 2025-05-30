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


		friend bool operator==(const AssetMetadata& lhs, const AssetMetadata& rhs)
		{
			return lhs.Handle == lhs.Handle;
		}

		static const AssetMetadata Invalid;
	};

	inline const AssetMetadata AssetMetadata::Invalid { AssetHandle::Invalid };
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::AssetMetadataHeader, SPHYNX_API)
SPX_REFLECT_PROPERTY(SphynxAsset)
SPX_REFLECT_PROPERTY(Type)
SPX_REFLECT_STRUCT_END(Sphynx::AssetMetadataHeader, SPHYNX_API)

SPX_REFLECT_STRUCT_BEGIN(Sphynx::AssetMetadata, SPHYNX_API)
SPX_REFLECT_PROPERTY(Handle)
SPX_REFLECT_PROPERTY(Type)
SPX_REFLECT_PROPERTY(Path)
SPX_REFLECT_PROPERTY(Dependencies)
SPX_REFLECT_STRUCT_END(Sphynx::AssetMetadata, SPHYNX_API)