#include "spxpch.h"
#include "AnimationAssetImporter.h"

#include "Asset/AssetMetadata.h"
#include "AnimationAsset.h"
#include "Asset/AssetManager.h"

#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Serialization/Reflection/ReflectionSerializer.h"
#include "Serialization/FileReader.h"
#include "Serialization/FileWriter.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> AnimationAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing animation2D: {}", metadata.Path.string().c_str());

		return Load(metadata);
	}

	std::shared_ptr<IAsset> AnimationAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading animation2D from {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		// read animation2D data from .spxasset file
		Animation2DAssetMetadata animationMetadata = AssetImporter::DeserializeAsset<Animation2DAssetMetadata>(metadata);

		Animation2D* animation = new Animation2D(animationMetadata.Sprites);

		// create animation2D asset object
		std::shared_ptr<Asset<Animation2D>> asset = std::make_shared<Asset<Animation2D>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = animation;

		return asset;
	}

	void AnimationAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving animation2D to {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		std::shared_ptr<Asset<Animation2D>> animationAsset = AssetManager::GetAsset<Animation2D>(metadata.Handle);

		// create a .spxasset file for each sprite in the spritesheet
		Animation2DAssetMetadata Animation2DAssetMetadata;
		for (AssetHandle& handle : animationAsset->Asset->Sprites)
		{
			SPX_CORE_ASSERT(!(handle == AssetHandle::Invalid), "Error! Sprite Asset Handle is invalid.");

			Animation2DAssetMetadata.Sprites.Add(handle);
		}

		// create .spxasset for the animation2D
		AssetImporter::SerializeAsset(metadata, Animation2DAssetMetadata);
	}
}
