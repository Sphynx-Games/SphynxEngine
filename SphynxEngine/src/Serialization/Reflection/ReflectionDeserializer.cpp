#include "spxpch.h"
#include "ReflectionDeserializer.h"

#include "Serialization/Reader.h"


namespace Sphynx
{
	void ReflectionDeserializer::Deserialize()
	{
		// read name and value for each property in the object
		for (const Reflection::Property& property : m_Class)
		{
			std::string name;
			m_Reader.Read(name);

			SPX_CORE_ASSERT(name == property.Name, "Error while desirializing!");

			m_Reader.Read((std::byte*)m_Obj + property.Offset, property.Type.Size);

			// TODO: manage properties of non primitive types (struct, enum, other types of classes...)
		}
	}

}