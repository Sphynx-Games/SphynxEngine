#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	template<typename T>
	struct SPHYNX_API AssetDependencySolver
	{
		void operator()(const T& t, AssetMetadata& metadata) {}
	};
}