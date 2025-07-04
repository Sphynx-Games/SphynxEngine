#include "spxpch.h"
#include "FontAssetImporter.h"

#include "Asset/AssetManager.h"
#include "FontAsset.h"
#include "Renderer/Renderer.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> FontAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing font: {}", metadata.Path.string().c_str());

		FontAssetMetadata fontMetadata;
		fontMetadata.RelativePath = path;
		AssetImporter::SerializeAsset(metadata, fontMetadata);

		return Load(metadata);
	}

	std::shared_ptr<IAsset> FontAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading font from {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		// read font data from .spxasset file
		FontAssetMetadata fontMetadata = AssetImporter::DeserializeAsset<FontAssetMetadata>(metadata);

		// create a font asset from a raw image file
		Font* font = FontLoader::Load(fontMetadata.RelativePath);

		std::shared_ptr<Asset<Font>> asset = std::make_shared<Asset<Font>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = font;

		return asset;
	}

	void FontAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving font to {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		std::shared_ptr<Asset<Font>> fontAsset = AssetManager::GetAsset<Font>(metadata.Handle);

		FontAssetMetadata fontMetadata;
		fontMetadata.RelativePath = fontAsset->RelativePath;
		AssetImporter::SerializeAsset(metadata, fontMetadata);
	}
}
