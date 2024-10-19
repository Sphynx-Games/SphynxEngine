#include "spxpch.h"
#include "TextureAssetImporter.h"
#include "Asset/AssetMetadata.h"
#include "TextureAsset.h"
#include "Renderer/Renderer.h"
#include "Logging/Log.h"
#include "Platform/SDL/SDLTexture.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Serialization/Reflection/ReflectionSerializer.h"
#include "Serialization/FileReader.h"
#include "Serialization/FileWriter.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> TextureAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing texture: {}", path.string().c_str());

		TextureAssetMetadata textureMetadata;
		textureMetadata.RelativePath = path;
		AssetImporter::SerializeAsset(metadata, textureMetadata);

		return Load(metadata);
	}

	std::shared_ptr<IAsset> TextureAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading texture from {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		// read texture data from .spxasset file
		TextureAssetMetadata textureMetadata = AssetImporter::DeserializeAsset<TextureAssetMetadata>(metadata);

		// create a texture asset from a raw image file
		Texture* texture = ImportFromFilePath(textureMetadata.RelativePath);

		std::shared_ptr<Asset<Texture>> asset = std::make_shared<Asset<Texture>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = texture;

		return asset;
	}

	void TextureAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving texture to {} file: {}", ASSET_EXTENSION, metadata.Path.string().c_str());

		std::shared_ptr<Asset<Texture>> textureAsset = AssetManager::GetAsset<Texture>(metadata.Handle);

		TextureAssetMetadata textureMetadata;
		textureMetadata.RelativePath = textureAsset->RelativePath;
		AssetImporter::SerializeAsset(metadata, textureMetadata);
	}

	Texture* TextureAssetImporter::ImportFromFilePath(const std::filesystem::path& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:     return SDLTextureLoader::Load(path);
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}
