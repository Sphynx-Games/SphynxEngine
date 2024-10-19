#pragma once

#include "Asset/AssetManager.h"
#include "Renderer/Font.h"
#include "FontAssetImporter.h"

#include <filesystem>


namespace Sphynx
{
	template<>
	struct SPHYNX_API Asset<Font> : public IAsset
	{
		virtual ~Asset()
		{
			Font*& font = Asset;
			if (font != nullptr)
			{
				delete font;
				font = nullptr;
			}
		}

		Font* Asset;

		inline operator Font* () { return Asset; }
		inline operator const Font* () const { return Asset; }

		virtual void* GetRawAsset() const override { return Asset; }

		std::filesystem::path RelativePath;
	};

	struct SPHYNX_API FontAssetMetadata
	{
		std::filesystem::path RelativePath;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Font);
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::FontAssetMetadata)

SPX_REFLECT_PROPERTY(RelativePath)

SPX_REFLECT_STRUCT_END(Sphynx::FontAssetMetadata)
