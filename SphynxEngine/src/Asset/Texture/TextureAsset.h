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
		Texture* Asset;

		inline operator Texture* () { return Asset; }
		inline operator const Texture* () const { return Asset; }

		virtual void* GetRawAsset() const override { return Asset; }

		std::filesystem::path RelativePath;
	};

	struct SPHYNX_API TextureAssetMetadata
	{
		std::filesystem::path RelativePath;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Texture);
}
