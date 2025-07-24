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

			enum class SPHYNX_API ValueType : uint8_t
			{
				VALUE,
				LVALUE_REFERENCE,
				RVALUE_REFERENCE,
			};

			Property(const Type& type, const char* name, size_t offset);
			Property(const Property& other);
			Property(Property&& other) noexcept;
			~Property();

			bool HasQualifier(Qualifier qualifier) const;
			bool IsConstant() const;
			bool IsVolatile() const;

			bool IsPublic() const;
			bool IsProtected() const;
			bool IsPrivate() const;

			bool IsValue() const;
			bool IsRValueReference() const;
			bool IsLValueReference() const;
			bool IsPointer() const;

			template<typename T>
			bool HasAttribute() const;

			template<typename T>
			const T* GetAttribute() const;

			const Type& Type;
			const char* Name;
			size_t Offset;
			uint8_t Qualifiers;
			AccessSpecifier AccessSpecifier;
			ValueType ValueType;
			size_t PointerIndirectionCount;

			std::vector<Attribute*> Attributes;
		};
		
		template<typename T>
		inline bool Property::HasAttribute() const 
		{ 
			return GetAttribute<T>() != nullptr; 
		}
		
		template<typename T>
		inline const T* Property::GetAttribute() const
		{
			size_t AttributesCount = Attributes.size();
			for (size_t i = 0; i < AttributesCount; ++i)
			{
				// TODO: change this ugly dynamic_cast
				if (const T* attr = dynamic_cast<const T*>(Attributes[i]))
					return attr;
			}

			return nullptr;
		}
	}
}
