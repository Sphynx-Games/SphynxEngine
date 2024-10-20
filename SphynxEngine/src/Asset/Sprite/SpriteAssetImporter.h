#pragma once

#include "Core/Core.h"
#include "Asset/AssetImporter.h"
#include "Asset/Asset.h"

#include <memory>


namespace Sphynx
{
	struct AssetMetadata;
	class Sprite;

	class SPHYNX_API SpriteAssetImporter
	{
	public:
		static std::shared_ptr<IAsset> Import(const AssetMetadata& metadata, const std::filesystem::path& path);
		static std::shared_ptr<IAsset> Load(const AssetMetadata& metadata);
		static void Save(const AssetMetadata& metadata);

	private:
		static std::shared_ptr<IAsset> LoadSprite(class Reader& reader, const AssetMetadata& metadata);
		static std::shared_ptr<IAsset> LoadSpriteInSpritesheet(Reader& reader, const AssetMetadata& metadata);

	};
}
