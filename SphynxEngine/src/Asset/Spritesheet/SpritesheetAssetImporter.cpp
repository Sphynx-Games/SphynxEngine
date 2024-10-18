#include "spxpch.h"
#include "SpritesheetAssetImporter.h"
#include "SpritesheetAsset.h"
#include "Asset/Sprite/SpriteAsset.h"
#include "Asset/Texture/TextureAsset.h"
#include "Asset/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/Sprite.h"
#include "Renderer/Texture.h"
#include "Logging/Log.h"
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
		SPX_CORE_LOG_TRACE("Loading spritesheet from .spxasset file: {}", metadata.Path.string().c_str());

		SPX_CORE_ASSERT(metadata.Dependencies.Size() == 1, "Error! SpritesheetAsset must have only one dependency.");

		AssetHandle dependencyHandle = metadata.Dependencies[0];
		SPX_CORE_ASSERT(AssetManager::GetAssetType(dependencyHandle) == TypeToAssetType<Texture>::Value, "Error! SpritesheetAsset must depend on a Texture.");

		// read spritesheet data from .spxasset file
		SpritesheetAssetMetadata spritesheetMetadata;

		FileReader reader(metadata.Path);

		size_t numSprites;
		reader.Read(numSprites);
		for (size_t i = 0; i < numSprites; ++i)
		{
			AssetHandle handle;
			reader.Read(handle);
			spritesheetMetadata.SpritesHandles.Add(handle);
		}
		//ReflectionDeserializer deserializer(spritesheetMetadata, reader); // TODO: implement container deserialization in the reflection deserializer 
		//deserializer.Deserialize();

		// set spritesheet values
		std::shared_ptr<Asset<Texture>> dependencyAsset = AssetManager::GetAsset<Texture>(dependencyHandle);

		Spritesheet* spritesheet = new Spritesheet();
		spritesheet->m_Texture = dependencyAsset->Asset;

		for (AssetHandle handle : spritesheetMetadata.SpritesHandles)
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
		SPX_CORE_LOG_TRACE("Saving spritesheet to .spxasset file: {}", metadata.Path.string().c_str());

		std::shared_ptr<Asset<Spritesheet>> spritesheetAsset = AssetManager::GetAsset<Spritesheet>(metadata.Handle);

		SpritesheetAssetMetadata spritesheetMetadata;

		// create directory to store the .spxasset files for the sprites of the spritesheet
		uint8_t i = 0;
		std::filesystem::path spritesDirectory = metadata.Path;
		spritesDirectory.replace_extension();
		spritesDirectory += "_Sprites";
		std::filesystem::create_directories(spritesDirectory);

		// create a .spxasset file for each sprite in the spritesheet
		for (Sprite* sprite : spritesheetAsset->Asset->m_Sprites)
		{
			AssetHandle spriteHandle = AssetManager::GetAssetHandleFromAddress(sprite);
			std::shared_ptr<Asset<Sprite>> spriteAsset;

			if (spriteHandle == AssetHandle::Invalid)
			{
				std::filesystem::path spritePath = spritesDirectory;
				spritePath += ("\\sprite" + std::to_string(i));
				spritePath.replace_extension(".spxasset");
				spriteAsset = AssetManager::RegisterAsset<Sprite>(sprite, spritePath);
				++i;
			}
			spriteHandle = spriteAsset->Handle;
			SPX_CORE_ASSERT(!(spriteHandle == AssetHandle::Invalid), "Error! Sprite Asset Handle is invalid.");
			spritesheetMetadata.SpritesHandles.Add(spriteHandle);
		}

		// create .spxasset for the spritesheet
		FileWriter writer(metadata.Path);
		writer.Write(spritesheetMetadata.SpritesHandles.Size());
		for (AssetHandle handle : spritesheetMetadata.SpritesHandles)
		{
			writer.Write(handle);
		}
		//ReflectionSerializer serializer(spritesheetMetadata, writer); // TODO: implement container serialization in the reflection serializer 
		//serializer.Serialize();
	}
}
