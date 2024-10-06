#include "spxpch.h"
#include "SpriteAssetImporter.h"

#include "Asset/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/Sprite.h"
#include "Logging/Log.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Serialization/FileReader.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> SpriteAssetImporter::Import(const AssetMetadata& metadata)
	{
		/*SPX_CORE_LOG_TRACE("Importing sprite: {}", metadata.Path.string().c_str());
		
		Sprite* sprite = new Sprite();

		FileReader reader = FileReader(metadata.Path);
		ReflectionDeserializer deserializer = ReflectionDeserializer(sprite, reader);
		deserializer.Deserialize();

		sprite->m_Texture = AssetManager::GetAsset<Texture>(sprite->m_Texture->Handle);

		std::shared_ptr<Asset<Sprite>> asset = std::make_shared<Asset<Sprite>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = sprite;

		return asset;*/
		return nullptr;
	}

	/*Texture* SpriteAssetImporter::Load(const std::filesystem::path& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:     return SDLTextureLoader::Load(path);
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}*/
}
