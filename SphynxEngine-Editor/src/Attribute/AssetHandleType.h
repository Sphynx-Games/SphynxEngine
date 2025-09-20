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
			Sphynx::AssetType Type;
		};
	}
}
