#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Asset.h"

#include <filesystem>


namespace Sphynx
{
	using AssetHandle = UUID;

	struct AssetMetadata
	{
		AssetHandle Handle;
		AssetType Type;
		std::filesystem::path Path;
	};
}
