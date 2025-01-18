#pragma once

#include "Asset/AssetManager.h"
#include "Renderer/Texture.h"
#include "TextureAssetImporter.h"

#include <filesystem>


namespace Sphynx
{
	template<>
	struct SPHYNX_API Asset<Texture> : public IAsset
	{
	public:
		virtual ~Asset()
		{
			Texture*& texture = Asset;
			if (texture != nullptr)
			{
				delete texture;
				texture = nullptr;
			}
		}

		inline operator Texture* () { return Asset; }
		inline operator const Texture* () const { return Asset; }

		virtual void* GetRawAsset() const override { return Asset; }

	public:
		Texture* Asset;
		std::filesystem::path RelativePath;
	};

	struct SPHYNX_API TextureAssetMetadata
	{
		std::filesystem::path RelativePath;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Texture);
}

SPX_REFLECT_STRUCT_BEGIN(Sphynx::TextureAssetMetadata)

	SPX_REFLECT_PROPERTY(RelativePath)

SPX_REFLECT_STRUCT_END(Sphynx::TextureAssetMetadata)
