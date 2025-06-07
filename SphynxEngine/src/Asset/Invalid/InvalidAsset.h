#pragma once

#include "Core/Core.h"
#include "Asset/Asset.h"


namespace Sphynx
{
	struct SPHYNX_API Invalid {};

	template<>
	struct SPHYNX_API Asset<Invalid> : public IAsset {};

	//GENERATE_ASSETTYPE_CONVERSOR(Invalid);
}

#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT(Sphynx::Invalid, SPHYNX_API)
