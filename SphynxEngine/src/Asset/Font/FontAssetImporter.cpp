#include "spxpch.h"
#include "FontAssetImporter.h"
#include "Asset/AssetMetadata.h"
#include "FontAsset.h"
#include "Renderer/Renderer.h"
#include "Logging/Log.h"
#include "Platform/SDL/SDLFont.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Serialization/Reflection/ReflectionSerializer.h"
#include "Serialization/FileReader.h"
#include "Serialization/FileWriter.h"


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
		Font* font = ImportFromFilePath(fontMetadata.RelativePath);

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

	Font* FontAssetImporter::ImportFromFilePath(const std::filesystem::path& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:     return SDLFontLoader::Load(path);
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}
