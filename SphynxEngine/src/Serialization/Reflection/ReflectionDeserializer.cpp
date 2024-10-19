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

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(m_Type);
		// Treat Plain Old Data as directly copyable
		using POD = Reflection::CommonAttribute::POD;
		if (const POD* pod = rClass.GetAttribute<POD>())
		{	
			std::vector<std::byte> buffer; buffer.resize(m_Type.Size);
			m_Reader.Read(buffer.data(), m_Type.Size);

			pod->CopyTo(buffer.data(), m_Obj);
			return;
		}

		using IndexedCollection = Reflection::CommonAttribute::IndexedCollection;
		if (const IndexedCollection* indexedCollection = rClass.GetAttribute<IndexedCollection>())
		{
			size_t collectionSize{ 0 };
			m_Reader.Read(collectionSize);

			for (size_t i = 0; i < collectionSize; ++i)
			{
				ReflectionDeserializer deserializer{ indexedCollection->Add(m_Obj),  indexedCollection->GetValueType(), m_Reader };
				deserializer.Deserialize();
			}
		}
		else
		{
			// Read name and value for each property in the object
			for (const Reflection::Property& property : rClass)
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

}