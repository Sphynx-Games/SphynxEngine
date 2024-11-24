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
		ReflectionSerializer::Serialize(m_Obj, m_Type, m_Writer);
	}
}