#include "Reflection/Property.h"
#include "Reflection/Type.h"

namespace Sphynx
{
	namespace Reflection
	{
		Property::Property(const ::Sphynx::Reflection::Type& type, const char* name, size_t offset) :
			Type(type),
			Name(name),
			Offset(offset),
			Attributes()
		{

		}

		Property::Property(const Property& other) :
			Type(other.Type),
			Name(other.Name),
			Offset(other.Offset),
			Attributes(other.Attributes)
		{

		}

		Property::Property(Property&& other) noexcept :
			Type(other.Type),
			Name(other.Name),
			Offset(std::move(other.Offset)),
			Attributes(std::move(other.Attributes))
		{

		}

		Property::~Property()
		{
			Name = nullptr;
			Offset = 0;
			Attributes.clear();
		}

	}
}
