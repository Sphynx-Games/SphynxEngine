#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Traits/Utility.h"


#define TYPE_TO_ASSETTYPE(Type) #Type
#define GENERATE_ASSETTYPE_CONVERSOR(Type) \
	template<> \
	struct SPHYNX_API TypeToAssetType<Type> \
	{ \
		inline static const ::Sphynx::AssetType Value = TYPE_TO_ASSETTYPE(Type); \
	}


namespace Sphynx
{
	using AssetHandle = Traits::unique_type<UUID, struct __AssetHandle__>;
	using AssetType = Traits::unique_type<std::string, struct __AssetType__>;

	struct SPHYNX_API IAsset
	{
		AssetHandle Handle;
		AssetType Type;

		virtual ~IAsset() = default;
		virtual void* GetRawAsset() const = 0;
	};

	template<typename T>
	struct Asset : public IAsset
	{
		virtual ~Asset()
		{
			T*& asset = Asset;
			if (asset != nullptr)
			{
				delete asset;
				asset = nullptr;
			}
		}

		T* Asset;

		inline operator T*() { return Asset; }
		inline operator const T*() const { return Asset; }

		virtual void* GetRawAsset() const override { return Asset; }
	};

	template<typename T>
	struct SPHYNX_API TypeToAssetType;
}
