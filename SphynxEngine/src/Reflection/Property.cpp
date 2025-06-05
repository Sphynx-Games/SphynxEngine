#include "Reflection/Property.h"
#include "Reflection/Type.h"
#include <iostream>

namespace Sphynx
{
	namespace Reflection
	{
		Property::Property(const ::Sphynx::Reflection::Type& type, const char* name, size_t offset, uint8_t qualifiers, AccessSpecifier accessSpecifier) :
			Type(type),
			Name(name),
			Offset(offset),
			Qualifiers(qualifiers),
			AccSpecifier(accessSpecifier),
			Attributes()
		{

		}

		Property::Property(const Property& other) :
			Type(other.Type),
			Name(other.Name),
			Offset(other.Offset),
			Qualifiers(other.Qualifiers),
			AccSpecifier(other.AccSpecifier),
			Attributes(other.Attributes)
		{

		}

		Property::Property(Property&& other) noexcept :
			Type(other.Type),
			Name(other.Name),
			Offset(std::move(other.Offset)),
			Qualifiers(std::move(other.Qualifiers)),
			AccSpecifier(std::move(other.AccSpecifier)),
			Attributes(std::move(other.Attributes))
		{

		}

		Property::~Property()
		{
			Name = nullptr;
			Offset = 0;
			Qualifiers = 0;
			Attributes.clear();
		}

		bool Property::HasQualifier(Qualifier qualifier) const
		{
			return (Qualifiers & static_cast<typename std::underlying_type<Qualifier>::type>(qualifier)) != 0;
		}

		bool Property::IsConstant() const
		{
			return HasQualifier(Qualifier::CONSTANT);
		}

		bool Property::IsVolatile() const
		{
			return HasQualifier(Qualifier::VOLATILE);
		}

		bool Property::IsPublic() const
		{
			return AccSpecifier == AccessSpecifier::PUBLIC;
		}

		bool Property::IsProtected() const
		{
			return AccSpecifier == AccessSpecifier::PROTECTED;
		}

		bool Property::IsPrivate() const
		{
			return AccSpecifier == AccessSpecifier::PRIVATE;
		}

	}
}
