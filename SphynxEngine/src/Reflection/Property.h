#pragma once

#include "Core/Core.h"
#include "AccessSpecifier.h"
#include "QualifiedType.h"
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
			Property(const QualifiedType& qualifiedType, const char* name, size_t offset);
			Property(const Property& other);
			Property(Property&& other) noexcept;
			~Property();

			const Type& GetType() const;

			bool HasQualifier(Qualifier::Value qualifier) const;
			bool IsConstant() const;
			bool IsVolatile() const;

			bool IsPublic() const;
			bool IsProtected() const;
			bool IsPrivate() const;

			bool IsValue() const;
			bool IsRValueReference() const;
			bool IsLValueReference() const;
			bool IsPointer() const;

			size_t GetPointerIndirection() const;

			template<typename T>
			bool HasAttribute() const;

			template<typename T>
			const T* GetAttribute() const;

			QualifiedType QualifiedType;
			const char* Name;
			size_t Offset;
			AccessSpecifier AccessSpecifier;

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
