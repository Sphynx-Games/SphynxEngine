#pragma once

#include "Core/Core.h"
#include "Asset/AssetImporter.h"
#include "Asset/Asset.h"

#include <memory>


namespace Sphynx
{
	struct AssetMetadata;
	class Spritesheet;

	class SPHYNX_API SpritesheetAssetImporter
	{
	public:
		static std::shared_ptr<IAsset> Import(const AssetMetadata& metadata, const std::filesystem::path& path);
		static std::shared_ptr<IAsset> Load(const AssetMetadata& metadata);
		static void Save(const AssetMetadata& metadata);
	};
}
