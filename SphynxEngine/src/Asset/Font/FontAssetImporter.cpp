#include "spxpch.h"
#include "FontAssetImporter.h"

#include "Renderer/Renderer.h"
#include "Logging/Log.h"
//#include "Renderer/Font.h"
#include "Platform/SDL/SDLFont.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> FontAssetImporter::Import(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Importing font: {}", metadata.Path.string().c_str());
		Font* font = Load(metadata.Path);

		std::shared_ptr<Asset<Font>> asset = std::make_shared<Asset<Font>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = font;

		return asset;
	}

	Font* FontAssetImporter::Load(const std::filesystem::path& path)
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
