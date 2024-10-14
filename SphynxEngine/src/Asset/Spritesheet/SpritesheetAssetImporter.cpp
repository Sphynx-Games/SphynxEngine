#include "spxpch.h"
#include "SpritesheetAssetImporter.h"
#include "SpritesheetAsset.h"
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
		/*SpritesheetAssetMetadata spritesheetMetadata;

		FileReader reader(metadata.Path);
		reader.Read(spritesheetMetadata.SpritesHandles);

		//ReflectionDeserializer deserializer(spritesheetMetadata, reader); // TODO: implement container deserialization in the reflection deserializer 
		//deserializer.Deserialize();

		Spritesheet* spritesheet = new Spritesheet();

		SPX_CORE_ASSERT(metadata.Dependencies.Size() == 1, "Error! SpritesheetAsset must have only one dependency.");

		AssetHandle dependencyHandle = metadata.Dependencies[0];
		SPX_CORE_ASSERT(AssetManager::GetAssetType(dependencyHandle) == "Texture", "Error! SpritesheetAsset must depend on a Texture."); // TODO: change this string typing

		std::shared_ptr<Asset<Texture>> dependencyAsset = AssetManager::GetAsset<Texture>(dependencyHandle);
		spritesheet->m_Texture = dependencyAsset;

		for (AssetHandle handle : spritesheetMetadata.SpritesHandles)
		{
			std::shared_ptr<Asset<Sprite>> spriteAsset = AssetManager::GetAsset<Sprite>(handle);
			spritesheet->m_Sprites.Add(spriteAsset->Asset);
		}

		std::shared_ptr<Asset<Spritesheet>> asset = std::make_shared<Asset<Spritesheet>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = spritesheet;

		return asset;*/
		return nullptr;
	}

	void SpritesheetAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving spritesheet to .spxasset file: {}", metadata.Path.string().c_str());

		std::shared_ptr<Asset<Spritesheet>> spritesheetAsset = AssetManager::GetAsset<Spritesheet>(metadata.Handle);

		SpritesheetAssetMetadata spritesheetMetadata;
		for (Sprite* sprite : spritesheetAsset->Asset->m_Sprites)
		{
			AssetHandle spriteHandle = AssetManager::GetAssetHandleFromAddress(sprite);
			SPX_CORE_ASSERT(!(spriteHandle == AssetHandle::Invalid), "Error! Sprite Asset Handle is invalid.");
			spritesheetMetadata.SpritesHandles.Add(spriteHandle);
		}

		FileWriter writer(metadata.Path);
		writer.Write(spritesheetMetadata.SpritesHandles);
		//ReflectionSerializer serializer(spritesheetMetadata, writer); // TODO: implement container serialization in the reflection serializer 
		//serializer.Serialize();
	}
}
