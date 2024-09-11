#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Traits/Utility.h"


namespace Sphynx
{
	using AssetHandle = Traits::unique_type<UUID, struct __AssetHandle__>;
	using AssetType = Traits::unique_type<std::string, struct __AssetType__>;

	struct SPHYNX_API IAsset
	{
		AssetHandle Handle;
		AssetType Type;
	};

	template<typename T>
	struct SPHYNX_API Asset : public IAsset
	{
		T* Asset;

		inline operator T*() { return Asset; }
		inline operator const T*() const { return Asset; }
	};
}
