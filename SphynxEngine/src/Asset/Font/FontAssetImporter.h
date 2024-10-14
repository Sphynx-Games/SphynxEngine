#pragma once

#include "Core/Core.h"
#include "Asset/AssetImporter.h"
#include "Asset/Asset.h"
#include "Asset/AssetMetadata.h"

#include <memory>


namespace Sphynx
{
	class Font;

	class SPHYNX_API FontAssetImporter
	{
	public:
		static std::shared_ptr<IAsset> Import(const AssetMetadata& metadata, const std::filesystem::path& path);
		static std::shared_ptr<IAsset> Load(const AssetMetadata& metadata);
		static void Save(const AssetMetadata& metadata);

	private:
		static class Font* ImportFromFilePath(const std::filesystem::path& path);
	};
}
