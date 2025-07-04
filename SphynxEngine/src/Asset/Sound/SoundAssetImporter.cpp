#include "spxpch.h"
#include "SoundAssetImporter.h"

#include "Asset/AssetManager.h"
#include "SoundAsset.h"
//#include "Logging/Log.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> SoundAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing sound: {}", metadata.Path.string().c_str());

		SoundAssetMetadata soundMetadata;
		soundMetadata.RelativePath = path;
		AssetImporter::SerializeAsset(metadata, soundMetadata);

		return Load(metadata);
	}

	std::shared_ptr<IAsset> SoundAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading sound from {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		// read sound data from .spxasset file
		SoundAssetMetadata soundMetadata = AssetImporter::DeserializeAsset<SoundAssetMetadata>(metadata);

		// create a sound asset from a raw sound file
		Sound* sound = SoundLoader::Load(soundMetadata.RelativePath);

		std::shared_ptr<Asset<Sound>> asset = std::make_shared<Asset<Sound>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = sound;

		return asset;
	}

	void SoundAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving sound to {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		std::shared_ptr<Asset<Sound>> soundAsset = AssetManager::GetAsset<Sound>(metadata.Handle);

		SoundAssetMetadata soundMetadata;
		soundMetadata.RelativePath = soundAsset->RelativePath;
		AssetImporter::SerializeAsset(metadata, soundMetadata);
	}
}
