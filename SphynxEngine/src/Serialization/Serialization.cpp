#include "spxpch.h"
#include "Serialization.h"


namespace Sphynx
{
	namespace Serialization
	{
		size_t CustomSerializer::GetTypeID() const
		{
			return Sphynx::Reflection::TypeID<CustomSerializer>::ID;
		}

		size_t CustomDeserializer::GetTypeID() const
		{
			return Sphynx::Reflection::TypeID<CustomDeserializer>::ID;
		}

	}
}
