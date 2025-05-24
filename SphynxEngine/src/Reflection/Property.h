#pragma once

#include "Core/Core.h"
#include <vector>
#include <cstdint>


namespace Sphynx
{
	namespace Reflection
	{
		struct Type;
		class Attribute;

		struct SPHYNX_API Property
		{
			Property(const Type& type, const char* name, size_t offset);
			Property(const Property& other);
			Property(Property&& other) noexcept;
			~Property();

			const Type& Type;
			const char* Name;
			size_t Offset;

			std::vector<Attribute*> Attributes;
		};
	}
}
