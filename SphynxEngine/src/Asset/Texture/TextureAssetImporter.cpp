#include "spxpch.h"
#include "TextureAssetImporter.h"

#include "Renderer/Renderer.h"
#include "Logging/Log.h"
#include "Platform/SDL/SDLTexture.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> TextureAssetImporter::Import(const AssetMetadata& metadata)
	{
		SPX_LOG_CORE_TRACE("Importing texture: {}", metadata.Path.string().c_str());
		Texture* texture = Load(metadata.Path);

		std::shared_ptr<Asset<Texture>> asset = std::make_shared<Asset<Texture>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = texture;

		return asset;
	}

	Texture* TextureAssetImporter::Load(const std::filesystem::path& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SPX_LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:     return SDLTextureLoader::Load(path);
		}

		SPX_LOG_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}
