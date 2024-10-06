#include "spxpch.h"
#include "ReflectionSerializer.h"

#include "Serialization/Writer.h"


namespace Sphynx
{
	void ReflectionSerializer::Serialize()
	{
		// write name and value for each property in the object
		for (const Reflection::Property& property : m_Class)
		{
			m_Writer.Write(property.Name);
			m_Writer.Write((std::byte*)m_Obj + property.Offset, property.Type.Size);

			// TODO: manage properties of non primitive types (struct, enum, other types of classes...)
		}
	}

}