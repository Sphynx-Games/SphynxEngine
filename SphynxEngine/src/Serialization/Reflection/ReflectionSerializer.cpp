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
				m_Writer.Write(*(const std::string*)m_Obj);
			}
			else if (&m_Type == &Reflection::GetType<std::wstring>())
			{
				m_Writer.Write(*(const std::wstring*)m_Obj);
			}
			else if (&m_Type == &Reflection::GetType<std::filesystem::path>())
			{
				m_Writer.Write(*(const std::filesystem::path*)m_Obj);
			}
			else
			{
				m_Writer.Write((const std::byte*)m_Obj, m_Type.Size);
			}
			return;
		}

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(m_Type);
		using POD = Reflection::CommonAttribute::POD;
		// Treat Plain Old Data as directly copyable
		if (const POD* pod = rClass.GetAttribute<POD>())
		{
			m_Writer.Write((const std::byte*)m_Obj, m_Type.Size);
			return;
		}
		using IndexedCollection = Reflection::CommonAttribute::IndexedCollection;
		using AssociativeCollection = Reflection::CommonAttribute::AssociativeCollection;
		if (const IndexedCollection* indexedCollection = rClass.GetAttribute<IndexedCollection>())
		{
			size_t collectionSize = indexedCollection->GetSize(m_Obj);
			m_Writer.Write(collectionSize);

			for (size_t i = 0; i < collectionSize; ++i)
			{
				ReflectionSerializer serializer{ indexedCollection->Get(m_Obj, i),  indexedCollection->GetValueType(), m_Writer };
				serializer.Serialize();
			}
		}
		else if (const AssociativeCollection* associativeCollection = rClass.GetAttribute<AssociativeCollection>())
		{
			size_t collectionSize = associativeCollection->GetSize(m_Obj);
			m_Writer.Write(collectionSize);

			for (size_t i = 0; i < collectionSize; ++i)
			{
				// Serialize key
				{
					ReflectionSerializer serializer{ associativeCollection->GetKey(m_Obj, i),  associativeCollection->GetKeyType(), m_Writer };
					serializer.Serialize();
				}
				// Serialize value
				{
					ReflectionSerializer serializer{ associativeCollection->GetValue(m_Obj, i),  associativeCollection->GetValueType(), m_Writer };
					serializer.Serialize();
				}
			}
		}
		else
		{
			// Write name and value for each property in the object
			for (const Reflection::Property& property : rClass)
			{
				// Write property name
				m_Writer.Write(property.Name);

				// Write property content recursively
				ReflectionSerializer serializer{ (const std::byte*)m_Obj + property.Offset, property.Type, m_Writer };
				serializer.Serialize();
			}
		}
	}

}