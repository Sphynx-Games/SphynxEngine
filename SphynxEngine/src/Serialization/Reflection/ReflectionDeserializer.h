#pragma once

#include "Core/Core.h"
#include "Reflection/Reflection.h"
#include "Serialization/Serialization.h"

#undef IndexedCollection
#undef AssociativeCollection
#undef POD

namespace Sphynx
{
	class SPHYNX_API ReflectionDeserializer
	{
	public:
		template<typename T>
		ReflectionDeserializer(T& obj, const class Reader& reader) :
			m_Obj(&obj),
			m_Type(Reflection::GetType<T>()),
			m_Reader(reader)
		{
		}

		ReflectionDeserializer(void* obj, const Reflection::Type& type, const class Reader& reader);

	public:
		template<typename T, typename TReader>
		static void Deserialize(T& obj, const TReader& reader);

		template<typename TReader>
		static void Deserialize(void* obj, const Reflection::Type& type, const TReader& reader);

		void Deserialize();

	private:
		void* m_Obj;
		const Reflection::Type& m_Type;
		const Reader& m_Reader;

	};

	template<typename T, typename TReader>
	inline void ReflectionDeserializer::Deserialize(T& obj, const TReader& reader)
	{
		Deserialize(&obj, Reflection::GetType<T>(), reader);
	}

	template<typename TReader>
	inline void ReflectionDeserializer::Deserialize(void* obj, const Reflection::Type& type, const TReader& reader)
	{
		if (type.Kind == Reflection::TypeKind::PRIMITIVE)
		{
			if (&type == &Reflection::GetType<bool>()) { reader.Read(*(bool*)obj); }
			else if (&type == &Reflection::GetType<char>()) { reader.Read(*(char*)obj); }
			else if (&type == &Reflection::GetType<wchar_t>()) { reader.Read(*(wchar_t*)obj); }
			else if (&type == &Reflection::GetType<int8_t>()) { reader.Read(*(int8_t*)obj); }
			else if (&type == &Reflection::GetType<int16_t>()) { reader.Read(*(int16_t*)obj); }
			else if (&type == &Reflection::GetType<int32_t>()) { reader.Read(*(int32_t*)obj); }
			else if (&type == &Reflection::GetType<int64_t>()) { reader.Read(*(int64_t*)obj); }
			else if (&type == &Reflection::GetType<uint8_t>()) { reader.Read(*(uint8_t*)obj); }
			else if (&type == &Reflection::GetType<uint16_t>()) { reader.Read(*(uint16_t*)obj); }
			else if (&type == &Reflection::GetType<uint32_t>()) { reader.Read(*(uint32_t*)obj); }
			else if (&type == &Reflection::GetType<uint64_t>()) { reader.Read(*(uint64_t*)obj); }
			else if (&type == &Reflection::GetType<float>()) { reader.Read(*(float*)obj); }
			else if (&type == &Reflection::GetType<double>()) { reader.Read(*(double*)obj); }
			// These are "advance primitive" data (special primitives)
			else if (&type == &Reflection::GetType<std::string>()) { reader.Read(*(std::string*)obj); }
			else if (&type == &Reflection::GetType<std::wstring>()) { reader.Read(*(std::wstring*)obj); }
			else if (&type == &Reflection::GetType<std::filesystem::path>()) { reader.Read(*(std::filesystem::path*)obj); }
			else { SPX_CORE_ASSERT(false, "Primitive type not handled"); }
			return;
		}

		if (type.Kind == Reflection::TypeKind::ENUM)
		{
			std::string name; reader.Read(name);
			const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(type);
			rEnum.SetName(obj, name);
			return;
		}

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(type);

		using CustomDeserializer = ::Sphynx::Serialization::CustomDeserializer<TReader>;
		if (const CustomDeserializer* deserializer = rClass.GetAttribute<CustomDeserializer>())
		{
			deserializer->Read(obj, reader);
			return;
		}

		if (reader.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			// Treat Plain Old Data as directly copyable
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				void* data = type.Alloc();
				reader.Read(data, type.Size);
				pod->CopyTo(data, obj);
				type.Dealloc(data);
				return;
			}
		}
		using IndexedCollection = Reflection::CommonAttribute::IndexedCollection;
		using AssociativeCollection = Reflection::CommonAttribute::AssociativeCollection;
		if (const IndexedCollection* indexedCollection = rClass.GetAttribute<IndexedCollection>())
		{
			size_t collectionSize = reader.PushSequence();
			for (size_t i = 0; i < collectionSize; ++i)
			{
				reader.PushIndex(i);
				ReflectionDeserializer::Deserialize(indexedCollection->Add(obj), indexedCollection->GetValueType(), reader);
				reader.PopIndex();
			}
			reader.PopSequence();
		}
		else if (const AssociativeCollection* associativeCollection = rClass.GetAttribute<AssociativeCollection>())
		{
			size_t collectionSize = reader.PushMap();
			for (size_t i = 0; i < collectionSize; ++i)
			{
				void* kBytes = associativeCollection->GetKeyType().Alloc();
				void* vBytes = associativeCollection->GetValueType().Alloc();
				// Deserialize key
				{
					reader.PushKey(i);
					ReflectionDeserializer::Deserialize(kBytes, associativeCollection->GetKeyType(), reader);
					reader.PopKey();
				}
				// Deserialize value
				{
					reader.PushValue(i);
					ReflectionDeserializer::Deserialize(vBytes, associativeCollection->GetValueType(), reader);
					reader.PopValue();
				}

				associativeCollection->Add(obj, kBytes, vBytes);

				associativeCollection->GetKeyType().Dealloc(kBytes);
				associativeCollection->GetValueType().Dealloc(vBytes);
			}
			reader.PopMap();
		}
		else
		{
			// Read name and value for each property in the object
			size_t size = reader.PushMap();
			for (size_t i = 0; i < size; ++i)
			{
				std::string name;
				reader.PushKey(i);
				reader.Read(name);
				reader.PopKey();

				auto it = std::find_if(std::begin(rClass), std::end(rClass), [&name](const Reflection::Property& p) { return p.Name == name; });
				if (it == std::end(rClass))
					continue;

				const Reflection::Property& property = *it;
				reader.PushValue(i);
				ReflectionDeserializer::Deserialize((std::byte*)obj + property.Offset, property.Type, reader);
				reader.PopValue();
			}
			reader.PopMap();
		}
	}
}