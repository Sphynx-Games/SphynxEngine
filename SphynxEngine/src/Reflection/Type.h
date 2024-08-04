#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct SPHYNX_API Type
		{
			const char* Name;
			size_t Size;
			size_t Alignment;
		};
	}
}
