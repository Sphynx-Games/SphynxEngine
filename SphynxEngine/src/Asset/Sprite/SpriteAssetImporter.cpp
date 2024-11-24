#include "spxpch.h"
#include "SpriteAssetImporter.h"
#include "Asset/AssetMetadata.h"
#include "SpriteAsset.h"
#include "Asset/Texture/TextureAsset.h"
#include "Asset/Spritesheet/SpritesheetAsset.h"
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
	std::shared_ptr<IAsset> SpriteAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing sprite: {}", path.string().c_str());

		return Load(metadata);
	}

	std::shared_ptr<IAsset> SpriteAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading sprite from {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		SPX_CORE_ASSERT(metadata.Dependencies.Size() == 1, "Error! SpriteAsset must have only one dependency.");

		// read sprite data from .spxasset file
		// (NOTE: we need to deserialize each metadata individually in oder to be able to perform some checks)
		FileReader reader(metadata.Path);
		SPX_CORE_ASSERT(reader.IsValid(), "Could not open file: {} !!", metadata.Path);

		AssetMetadataHeader header = AssetImporter::DeserializeAssetHeader(reader);

		if (header.Type == TypeToAssetType<Spritesheet>::Value)
		{
			return LoadSpriteInSpritesheet(reader, metadata);
		}
		else if(header.Type == TypeToAssetType<Sprite>::Value)
		{
			return LoadSprite(reader, metadata);
		}
		else
		{
			return nullptr;
		}
	}

	void SpriteAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving sprite to {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		std::shared_ptr<Asset<Sprite>> spriteAsset = AssetManager::GetAsset<Sprite>(metadata.Handle);

		SpriteAssetMetadata spriteMetadata;
		if (spriteAsset != nullptr)
		{
			spriteMetadata.Position = spriteAsset->Asset->GetPosition();
			spriteMetadata.Size = spriteAsset->Asset->GetSize();
			spriteMetadata.Pivot = spriteAsset->Asset->GetPivot();
			spriteMetadata.PixelsPerUnit = spriteAsset->Asset->GetPixelsPerUnit();
		}

		AssetImporter::SerializeAsset(metadata, spriteMetadata);
	}

	std::shared_ptr<IAsset> SpriteAssetImporter::LoadSprite(const Reader& reader, const AssetMetadata& metadata)
	{
		SpriteAssetMetadata spriteMetadata = AssetImporter::DeserializeAssetMetadata<SpriteAssetMetadata>(reader);

		// set sprite values
		Sprite* sprite = new Sprite();
		sprite->SetPosition(spriteMetadata.Position);
		sprite->SetSize(spriteMetadata.Size);
		sprite->SetPivot(spriteMetadata.Pivot);
		sprite->SetPixelsPerUnit(spriteMetadata.PixelsPerUnit);

		AssetHandle dependencyHandle = metadata.Dependencies[0];
		std::shared_ptr<IAsset> dependencyAsset = AssetManager::GetAsset(dependencyHandle);
		if (AssetManager::GetAssetType(dependencyHandle) == TypeToAssetType<Texture>::Value)
		{
			sprite->m_Texture = std::static_pointer_cast<Asset<Texture>>(dependencyAsset)->Asset;
		}
		else if (AssetManager::GetAssetType(dependencyHandle) == TypeToAssetType<Spritesheet>::Value)
		{
			std::shared_ptr<Asset<Spritesheet>> spritesheet = std::static_pointer_cast<Asset<Spritesheet>>(dependencyAsset);
			sprite->m_Texture = spritesheet->Asset->m_Texture;
		}

		// create sprite asset object
		std::shared_ptr<Asset<Sprite>> asset = std::make_shared<Asset<Sprite>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = sprite;

		return asset;
	}

	std::shared_ptr<IAsset> SpriteAssetImporter::LoadSpriteInSpritesheet(const Reader& reader, const AssetMetadata& metadata)
	{
		// check if the spritesheet depends on a texture
		AssetHandle dependencyHandle = metadata.Dependencies[0];
		SPX_CORE_ASSERT(AssetManager::GetAssetType(dependencyHandle) == TypeToAssetType<Texture>::Value, "Error! SpritesheetAsset must depend on a Texture.");

		std::shared_ptr<Asset<Texture>> dependencyAsset = AssetManager::GetAsset<Texture>(dependencyHandle);

		// deserialize the spritesheet, but only load into memory the requested the sprite
		SpritesheetAssetMetadata spritesheetMetadata = AssetImporter::DeserializeAssetMetadata<SpritesheetAssetMetadata>(reader);

		Sprite* sprite = nullptr;
		for (auto& [spriteHandle, spriteMetadata] : spritesheetMetadata.SpritesData)
		{
			if (spriteHandle == metadata.Handle)
			{
				sprite = new Sprite();
				sprite->SetPosition(spriteMetadata.Position);
				sprite->SetSize(spriteMetadata.Size);
				sprite->SetPivot(spriteMetadata.Pivot);
				sprite->SetPixelsPerUnit(spriteMetadata.PixelsPerUnit);
				sprite->m_Texture = std::static_pointer_cast<Asset<Texture>>(dependencyAsset)->Asset;

				break;
			}
		}

		SPX_CORE_ASSERT(sprite != nullptr, "Error! Sprite not found in spritesheet.");

		// create sprite asset object
		std::shared_ptr<Asset<Sprite>> asset = std::make_shared<Asset<Sprite>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = sprite;

		return asset;
	}
}
