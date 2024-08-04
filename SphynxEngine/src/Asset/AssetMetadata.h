#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Traits/Utility.h"
#include "Asset.h"

#include <filesystem>


namespace Sphynx
{
	using AssetHandle = UniqueType<UUID, struct __AssetHandle__>;

	struct AssetMetadata
	{
		AssetHandle Handle;
		AssetType Type;
		std::filesystem::path Path;
	};
}
