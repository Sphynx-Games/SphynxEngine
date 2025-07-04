#pragma once

#include "Core/Core.h"
#include "Asset/Asset.h"
#include "Sound/Sound.h"

#include <filesystem>


namespace Sphynx
{
	template<>
	struct Asset<Sound> : public IAsset
	{
	public:
		virtual ~Asset()
		{
			Sound*& sound = Asset;
			if (sound != nullptr)
			{
				delete sound;
				sound = nullptr;
			}
		}

		inline operator Sound* () { return Asset; }
		inline operator const Sound* () const { return Asset; }

		virtual void* GetRawAsset() const override { return Asset; }

	public:
		Sound* Asset;
		std::filesystem::path RelativePath;
	};

	struct SPHYNX_API SoundAssetMetadata
	{
		std::filesystem::path RelativePath;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Sound);
}

SPX_REFLECT_STRUCT_BEGIN(Sphynx::SoundAssetMetadata, SPHYNX_API)

	SPX_REFLECT_PROPERTY(RelativePath)

SPX_REFLECT_STRUCT_END(Sphynx::SoundAssetMetadata, SPHYNX_API)