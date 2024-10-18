#include "spxpch.h"
#include "SpriteAssetImporter.h"
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
		SPX_CORE_LOG_TRACE("Loading sprite from .spxasset file: {}", metadata.Path.string().c_str());

		SPX_CORE_ASSERT(metadata.Dependencies.Size() == 1, "Error! SpriteAsset must have only one dependency.");

		// read sprite data from .spxasset file
		SpriteAssetMetadata spriteMetadata;

		FileReader reader(metadata.Path);
		ReflectionDeserializer deserializer(spriteMetadata, reader);
		deserializer.Deserialize();

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

	void SpriteAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving sprite to .spxasset file: {}", metadata.Path.string().c_str());

		std::shared_ptr<Asset<Sprite>> spriteAsset = AssetManager::GetAsset<Sprite>(metadata.Handle);

		SpriteAssetMetadata spriteMetadata;
		spriteMetadata.Position = spriteAsset->Asset->GetPosition();
		spriteMetadata.Size = spriteAsset->Asset->GetSize();
		spriteMetadata.Pivot = spriteAsset->Asset->GetPivot();
		spriteMetadata.PixelsPerUnit = spriteAsset->Asset->GetPixelsPerUnit();

		FileWriter writer(metadata.Path);
		ReflectionSerializer serializer(spriteMetadata, writer);
		serializer.Serialize();
	}
}
