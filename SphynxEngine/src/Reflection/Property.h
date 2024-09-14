#pragma once

#include "Core/Core.h"
#include "Attribute.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct Class;
		struct Type;

		struct SPHYNX_API Property
		{
			const Type& Type;
			const char* Name;
			size_t Offset;

			std::vector<Attribute*> Attributes;
		};
	}
}
