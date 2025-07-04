#pragma once

//#include "Asset/AssetManager.h"
#include "Core/Core.h"
#include "Asset/Asset.h"
#include "Renderer/Font.h"
//#include "FontAssetImporter.h"

#include <filesystem>


namespace Sphynx
{
	template<>
	struct Asset<Font> : public IAsset
	{
	public:
		virtual ~Asset()
		{
			Font*& font = Asset;
			if (font != nullptr)
			{
				delete font;
				font = nullptr;
			}
		}

		inline operator Font* () { return Asset; }
		inline operator const Font* () const { return Asset; }

		virtual void* GetRawAsset() const override { return Asset; }

	public:
		Font* Asset;
		std::filesystem::path RelativePath;
	};

	struct SPHYNX_API FontAssetMetadata
	{
		std::filesystem::path RelativePath;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Font);
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::FontAssetMetadata, SPHYNX_API)

	SPX_REFLECT_PROPERTY(RelativePath)

SPX_REFLECT_STRUCT_END(Sphynx::FontAssetMetadata, SPHYNX_API)
