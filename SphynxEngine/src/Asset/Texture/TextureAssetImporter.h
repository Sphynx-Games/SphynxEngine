#pragma once

#include "Core/Core.h"
#include "Asset/AssetImporter.h"
#include "Asset/Asset.h"
#include "Asset/AssetMetadata.h"

#include <memory>


namespace Sphynx
{
	class SPHYNX_API TextureAssetImporter
	{
	public:
		static std::shared_ptr<IAsset> Import(const AssetMetadata& metadata);
		static class Texture* Load(const std::filesystem::path& path);

	};
}
