#include "spxpch.h"
#include "ReflectionSerializer.h"

#include "Serialization/Writer.h"


namespace Sphynx
{

	ReflectionSerializer::ReflectionSerializer(const void* obj, const Reflection::Type& type, Writer& writer) :
		m_Obj(obj),
		m_Type(type),
		m_Writer(writer)
	{
	}

	void ReflectionSerializer::Serialize()
	{
		if (m_Type.IsPrimitive)
		{
			// These are "advance primitive" data (special primitives)
			if (&m_Type == &Reflection::GetType<std::string>())
			{
				m_Writer.Write(*(std::string*)m_Obj);
			}
			else if (&m_Type == &Reflection::GetType<std::wstring>())
			{
				m_Writer.Write(*(std::wstring*)m_Obj);
			}
			else if (&m_Type == &Reflection::GetType<std::filesystem::path>())
			{
				m_Writer.Write(*(std::filesystem::path*)m_Obj);
			}
			else
			{
				m_Writer.Write((std::byte*)m_Obj, m_Type.Size);
			}
			return;
		}

		// Write name and value for each property in the object
		const Reflection::Class* rClass = static_cast<const Reflection::Class*>(&m_Type);
		for (const Reflection::Property& property : *rClass)
		{
			// Write property name
			m_Writer.Write(property.Name);

			// Write property content recursively
			ReflectionSerializer serializer{ (const std::byte*)m_Obj + property.Offset, property.Type, m_Writer };
			serializer.Serialize();
		}
	}

}