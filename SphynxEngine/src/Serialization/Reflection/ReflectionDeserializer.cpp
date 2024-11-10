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

		if (m_Type.IsEnum)
		{
			std::string name;
			m_Reader.Read(name);
			const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(m_Type);
			rEnum.SetName(m_Obj, name);
			return;
		}

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(m_Type);
		// Treat Plain Old Data as directly copyable
		using POD = Reflection::CommonAttribute::POD;
		if (const POD* pod = rClass.GetAttribute<POD>())
		{	
			void* data = m_Type.Alloc();
			m_Reader.Read(data, m_Type.Size);
			pod->CopyTo(data, m_Obj);
			m_Type.Dealloc(data);
			return;
		}

		using IndexedCollection = Reflection::CommonAttribute::IndexedCollection;
		using AssociativeCollection = Reflection::CommonAttribute::AssociativeCollection;
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
		else if (const AssociativeCollection* associativeCollection = rClass.GetAttribute<AssociativeCollection>())
		{
			size_t collectionSize{ 0 };
			m_Reader.Read(collectionSize);

			for (size_t i = 0; i < collectionSize; ++i)
			{
				void* kBytes = associativeCollection->GetKeyType().Alloc();
				void* vBytes = associativeCollection->GetValueType().Alloc();
				// Deserialize key
				{
					ReflectionDeserializer deserializer{ kBytes,  associativeCollection->GetKeyType(), m_Reader};
					deserializer.Deserialize();
				}
				// Deserialize value
				{
					ReflectionDeserializer deserializer{ vBytes,  associativeCollection->GetValueType(), m_Reader};
					deserializer.Deserialize();
				}

				associativeCollection->Add(m_Obj, kBytes, vBytes);

				associativeCollection->GetKeyType().Dealloc(kBytes);
				associativeCollection->GetValueType().Dealloc(vBytes);
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