#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	namespace Reflection
	{
		enum class SPHYNX_API AccessSpecifier : uint8_t
		{
			PUBLIC = 0,
			PROTECTED = 1,
			PRIVATE = 2,
		};
	}
}
