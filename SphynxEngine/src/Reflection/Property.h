#pragma once

#include "Core/Core.h"
#include "AccessSpecifier.h"
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
			enum class SPHYNX_API Qualifier : uint8_t
			{
				NONE		= 0,
				CONSTANT	= 1 << 0,
				VOLATILE	= 1 << 1,
				//MUTABLE 	= 1 << 2,
			};

			Property(const Type& type, const char* name, size_t offset, uint8_t qualifiers, AccessSpecifier accessSpecifier);
			Property(const Property& other);
			Property(Property&& other) noexcept;
			~Property();

			bool HasQualifier(Qualifier qualifier) const;
			bool IsConstant() const;
			bool IsVolatile() const;

			bool IsPublic() const;
			bool IsProtected() const;
			bool IsPrivate() const;

			const Type& Type;
			const char* Name;
			size_t Offset;
			uint8_t Qualifiers;
			AccessSpecifier AccSpecifier;

			std::vector<Attribute*> Attributes;
		};
	}
}
