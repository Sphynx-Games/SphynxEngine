#include "Reflection/Property.h"
#include "Reflection/Type.h"
#include <iostream>

namespace Sphynx
{
	namespace Reflection
	{
		Property::Property(const ::Sphynx::Reflection::Type& type, const char* name, size_t offset) :
			Type(type),
			Name(name),
			Offset(offset),
			Qualifiers(0),
			AccessSpecifier(AccessSpecifier::PUBLIC),
			ValueType(ValueType::VALUE),
			PointerIndirectionCount(0),
			Attributes()
		{

		}

		Property::Property(const Property& other) :
			Type(other.Type),
			Name(other.Name),
			Offset(other.Offset),
			Qualifiers(other.Qualifiers),
			AccessSpecifier(other.AccessSpecifier),
			ValueType(other.ValueType),
			PointerIndirectionCount(other.PointerIndirectionCount),
			Attributes(other.Attributes)
		{

		}

		Property::Property(Property&& other) noexcept :
			Type(other.Type),
			Name(other.Name),
			Offset(std::move(other.Offset)),
			Qualifiers(std::move(other.Qualifiers)),
			AccessSpecifier(std::move(other.AccessSpecifier)),
			ValueType(std::move(other.ValueType)),
			PointerIndirectionCount(std::move(other.PointerIndirectionCount)),
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
			return AccessSpecifier == AccessSpecifier::PUBLIC;
		}

		bool Property::IsProtected() const
		{
			return AccessSpecifier == AccessSpecifier::PROTECTED;
		}

		bool Property::IsPrivate() const
		{
			return AccessSpecifier == AccessSpecifier::PRIVATE;
		}

		bool Property::IsValue() const
		{
			return ValueType == ValueType::VALUE;
		}

		bool Property::IsRValueReference() const
		{
			return ValueType == ValueType::RVALUE_REFERENCE;
		}

		bool Property::IsLValueReference() const
		{
			return ValueType == ValueType::LVALUE_REFERENCE;
		}

		bool Property::IsPointer() const
		{
			return PointerIndirectionCount > 0;
		}
	}
}
