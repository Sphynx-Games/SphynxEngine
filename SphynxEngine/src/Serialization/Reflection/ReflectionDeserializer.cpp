#include "spxpch.h"
#include "ReflectionDeserializer.h"

#include "Serialization/Reader.h"


namespace Sphynx
{
	ReflectionDeserializer::ReflectionDeserializer(void* obj, const Reflection::Type& type, const Reader& reader) :
		m_Obj(obj),
		m_Type(type),
		m_Reader(reader)
	{
	}

	void ReflectionDeserializer::Deserialize()
	{
		Deserialize(m_Obj, m_Type, m_Reader);
	}

}