#include "spxpch.h"
#include "SpritesheetAssetImporter.h"

#include "Asset/AssetMetadata.h"
#include "SpritesheetAsset.h"
#include "Asset/Sprite/SpriteAsset.h"
#include "Asset/Texture/TextureAsset.h"
#include "Asset/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/Sprite.h"
#include "Renderer/Texture.h"

#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Serialization/Reflection/ReflectionSerializer.h"
#include "Serialization/FileReader.h"
#include "Serialization/FileWriter.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> SpritesheetAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing spritesheet: {}", metadata.Path.string().c_str());

		return Load(metadata);
	}

	std::shared_ptr<IAsset> SpritesheetAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading spritesheet from {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		SPX_CORE_ASSERT(metadata.Dependencies.Size() == 1, "Error! SpritesheetAsset must have only one dependency.");

		AssetHandle dependencyHandle = metadata.Dependencies[0];
		SPX_CORE_ASSERT(AssetManager::GetAssetType(dependencyHandle) == TypeToAssetType<Texture>::Value(), "Error! SpritesheetAsset must depend on a Texture.");

		// read spritesheet data from .spxasset file
		SpritesheetAssetMetadata spritesheetMetadata = AssetImporter::DeserializeAsset<SpritesheetAssetMetadata>(metadata);

		// set spritesheet values
		std::shared_ptr<Asset<Texture>> dependencyAsset = AssetManager::GetAsset<Texture>(dependencyHandle);

		Spritesheet* spritesheet = new Spritesheet();
		spritesheet->m_Texture = dependencyAsset->Asset;

		for (AssetHandle handle : spritesheetMetadata.SpritesData.GetKeys())
		{
			std::shared_ptr<Asset<Sprite>> spriteAsset = AssetManager::GetAsset<Sprite>(handle);
			spritesheet->m_Sprites.Add(spriteAsset->Asset);
		}

		// create spritesheet asset object
		std::shared_ptr<Asset<Spritesheet>> asset = std::make_shared<Asset<Spritesheet>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = spritesheet;

		return asset;
	}

	void SpritesheetAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving spritesheet to {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		std::shared_ptr<Asset<Spritesheet>> spritesheetAsset = AssetManager::GetAsset<Spritesheet>(metadata.Handle);

		// create a .spxasset file for each sprite in the spritesheet
		SpritesheetAssetMetadata spritesheetMetadata;
		for (Sprite* sprite : spritesheetAsset->Asset->m_Sprites)
		{
			AssetHandle spriteHandle = AssetManager::GetAssetHandleFromAddress(sprite);
			SPX_CORE_ASSERT(!(spriteHandle == AssetHandle::Invalid), "Error! Sprite Asset Handle is invalid.");

			std::shared_ptr<Asset<Sprite>> spriteAsset = AssetManager::GetAsset<Sprite>(spriteHandle);

			SpriteAssetMetadata spriteMetadata;
			spriteMetadata.Position = sprite->GetPosition();
			spriteMetadata.Size = sprite->GetSize();
			spriteMetadata.Pivot = sprite->GetPivot();
			spriteMetadata.PixelsPerUnit = sprite->GetPixelsPerUnit();

			spritesheetMetadata.SpritesData.Add(spriteHandle, spriteMetadata);
		}

		// create .spxasset for the spritesheet
		AssetImporter::SerializeAsset(metadata, spritesheetMetadata);
	}
}
