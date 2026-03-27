#pragma once

#include <Core/Core.h>
#include <Reflection/Attribute.h>
#include <Asset/Asset.h>


namespace Sphynx
{
	namespace EditorAttribute
	{
		struct AssetHandleType : public Reflection::Attribute
		{
			AssetHandleType(Sphynx::AssetType assetType) : Type(assetType) {};
			virtual size_t GetTypeID() const override;
			Sphynx::AssetType Type;
		};
	}
}

SPX_REGISTER_ATTRIBUTE(Sphynx::EditorAttribute::AssetHandleType);

inline size_t Sphynx::EditorAttribute::AssetHandleType::GetTypeID() const
{
	return Sphynx::Reflection::TypeID<Sphynx::EditorAttribute::AssetHandleType>::ID;
}
