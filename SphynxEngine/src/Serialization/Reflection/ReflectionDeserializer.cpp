#include "spxpch.h"
#include "ReflectionDeserializer.h"

#include "Serialization/Reader.h"


namespace Sphynx
{
	ReflectionDeserializer::ReflectionDeserializer(void* obj, const Reflection::Type& type, Reader& reader) :
		m_Obj(obj),
		m_Type(type),
		m_Reader(reader)
	{
	}

	void ReflectionDeserializer::Deserialize()
	{
		if (m_Type.IsPrimitive)
		{
			// These are "advance primitive" data (special primitives)
			if (&m_Type == &Reflection::GetType<std::string>())
			{
				m_Reader.Read(*(std::string*)m_Obj);
			}
			else if (&m_Type == &Reflection::GetType<std::wstring>())
			{
				m_Reader.Read(*(std::wstring*)m_Obj);
			}
			else if (&m_Type == &Reflection::GetType<std::filesystem::path>())
			{
				m_Reader.Read(*(std::filesystem::path*)m_Obj);
			}
			else
			{
				m_Reader.Read((std::byte*)m_Obj, m_Type.Size);
			}
			return;
		}

		// Read name and value for each property in the object
		const Reflection::Class* rClass = static_cast<const Reflection::Class*>(&m_Type);
		for (const Reflection::Property& property : *rClass)
		{
			// Read property name
			std::string name;
			m_Reader.Read(name);
			SPX_CORE_ASSERT(!strcmp(name.c_str(), property.Name), "Error while deserializing!");

			// Read property content recursively
			ReflectionDeserializer deserializer{ (std::byte*)m_Obj + property.Offset, property.Type, m_Reader };
			deserializer.Deserialize();
		}
	}

}