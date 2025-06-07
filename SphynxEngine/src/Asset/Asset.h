#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Traits/Utility.h"


#define TYPE_TO_ASSETTYPE(Type) {&::Sphynx::Reflection::GetType<Type>()}
#define GENERATE_ASSETTYPE_CONVERSOR(Type) \
	template<> \
	struct SPHYNX_API TypeToAssetType<Type> \
	{ \
		static ::Sphynx::AssetType Value() { return TYPE_TO_ASSETTYPE(Type); } \
	}

namespace Sphynx
{
	using AssetHandle = Traits::unique_type<UUID, struct __AssetHandle__>;
	struct SPHYNX_API AssetType
	{
		const Reflection::Type* Type = nullptr;

		bool operator==(const AssetType& other) const;
		bool operator<(const AssetType& other) const;

	private:
		friend struct std::hash<UUID>;
	};

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

template<>
struct std::hash<Sphynx::AssetType>
{
	std::size_t operator()(const Sphynx::AssetType& assetType) const
	{
		return std::hash<uintptr_t>()((uintptr_t)assetType.Type);
	}
};


#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::AssetType, SPHYNX_API)

SPX_REFLECT_ATTRIBUTE(Sphynx::Serialization::CustomSerializer<Sphynx::Writer>, Sphynx::Reflection::details::Tag<Sphynx::AssetType>{})
SPX_REFLECT_ATTRIBUTE(Sphynx::Serialization::CustomDeserializer<Sphynx::Reader>, Sphynx::Reflection::details::Tag<Sphynx::AssetType>{})
SPX_REFLECT_ATTRIBUTE(Sphynx::Serialization::CustomSerializer<Sphynx::YAMLWriter>, Sphynx::Reflection::details::Tag<Sphynx::AssetType>{})
SPX_REFLECT_ATTRIBUTE(Sphynx::Serialization::CustomDeserializer<Sphynx::YAMLReader>, Sphynx::Reflection::details::Tag<Sphynx::AssetType>{})

SPX_REFLECT_STRUCT_END(Sphynx::AssetType, SPHYNX_API)
