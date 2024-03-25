#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"


namespace Sphynx
{
	using AssetHandle = UUID;
	using AssetType = std::string;

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
