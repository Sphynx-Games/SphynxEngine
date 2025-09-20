#include "Reflection/Property.h"
#include "Reflection/Type.h"
#include <iostream>

namespace Sphynx
{
	namespace Reflection
	{
		Property::Property(const ::Sphynx::Reflection::Type& type, const char* name, size_t offset) :
			QualifiedType({ type, 0, ValueType::VALUE, 0 }),
			Name(name),
			Offset(offset),
			AccessSpecifier(AccessSpecifier::PUBLIC),
			Attributes()
		{

		}

		Property::Property(const ::Sphynx::Reflection::QualifiedType& qualifiedType, const char* name, size_t offset) :
			QualifiedType(qualifiedType),
			Name(name),
			Offset(offset),
			AccessSpecifier(AccessSpecifier::PUBLIC),
			Attributes()
		{

		}

		Property::Property(const Property& other) :
			QualifiedType(other.QualifiedType),
			Name(other.Name),
			Offset(other.Offset),
			AccessSpecifier(other.AccessSpecifier),
			Attributes(other.Attributes)
		{

		}

		Property::Property(Property&& other) noexcept :
			QualifiedType(other.QualifiedType),
			Name(other.Name),
			Offset(std::move(other.Offset)),
			AccessSpecifier(std::move(other.AccessSpecifier)),
			Attributes(std::move(other.Attributes))
		{

		}

		Property::~Property()
		{
			Name = nullptr;
			Offset = 0;
			Attributes.clear();
		}

		const Type& Property::GetType() const
		{
			return QualifiedType.Type;
		}

		bool Property::HasQualifier(Qualifier::Value qualifier) const
		{
			return (QualifiedType.Qualifiers & static_cast<Qualifier::Mask>(qualifier)) != 0;
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
			return QualifiedType.ValueType == ValueType::VALUE;
		}

		bool Property::IsRValueReference() const
		{
			return QualifiedType.ValueType == ValueType::RVALUE_REFERENCE;
		}

		bool Property::IsLValueReference() const
		{
			return QualifiedType.ValueType == ValueType::LVALUE_REFERENCE;
		}

		bool Property::IsPointer() const
		{
			return QualifiedType.PointerIndirectionCount > 0;
		}

		size_t Property::GetPointerIndirection() const
		{
			return QualifiedType.PointerIndirectionCount;
		}

	}
}
