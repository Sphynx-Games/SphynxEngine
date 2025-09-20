#pragma once

#include "Core/Core.h"
#include "Reflection/Reflection.h"
#include "Serialization/Serialization.h"

#undef IndexedCollection
#undef AssociativeCollection
#undef POD

namespace Sphynx
{
	class SPHYNX_API ReflectionSerializer
	{
	public:
		template<typename T>
		ReflectionSerializer(const T& obj, class Writer& writer) :
			m_Obj(&obj),
			m_Type(Reflection::GetType<T>()),
			m_Writer(writer)
		{
		}

		ReflectionSerializer(const void* obj, const Reflection::Type& type, class Writer& writer);

	public:
		template<typename T, typename TWriter>
		static void Serialize(const T& obj, TWriter& writer);

		template<typename TWriter>
		static void Serialize(const void* obj, const Reflection::Type& type, TWriter& write);

		void Serialize();

	private:
		const void* m_Obj;
		const Reflection::Type& m_Type;
		Writer& m_Writer;

	};

	template<typename T, typename TWriter>
	inline void ReflectionSerializer::Serialize(const T& obj, TWriter& writer)
	{
		Serialize(&obj, Reflection::GetType<T>(), writer);
	}

	template<typename TWriter>
	inline void ReflectionSerializer::Serialize(const void* obj, const Reflection::Type& type, TWriter& writer)
	{
		if (type.Kind == Reflection::TypeKind::PRIMITIVE)
		{
			if (&type == &Reflection::GetType<bool>()) { writer.Write(*(const bool*)obj); }
			else if (&type == &Reflection::GetType<char>()) { writer.Write(*(const char*)obj); }
			else if (&type == &Reflection::GetType<wchar_t>()) { writer.Write(*(const wchar_t*)obj); }
			else if (&type == &Reflection::GetType<int8_t>()) { writer.Write(*(const int8_t*)obj); }
			else if (&type == &Reflection::GetType<int16_t>()) { writer.Write(*(const int16_t*)obj); }
			else if (&type == &Reflection::GetType<int32_t>()) { writer.Write(*(const int32_t*)obj); }
			else if (&type == &Reflection::GetType<int64_t>()) { writer.Write(*(const int64_t*)obj); }
			else if (&type == &Reflection::GetType<uint8_t>()) { writer.Write(*(const uint8_t*)obj); }
			else if (&type == &Reflection::GetType<uint16_t>()) { writer.Write(*(const uint16_t*)obj); }
			else if (&type == &Reflection::GetType<uint32_t>()) { writer.Write(*(const uint32_t*)obj); }
			else if (&type == &Reflection::GetType<uint64_t>()) { writer.Write(*(const uint64_t*)obj); }
			else if (&type == &Reflection::GetType<float>()) { writer.Write(*(const float*)obj); }
			else if (&type == &Reflection::GetType<double>()) { writer.Write(*(const double*)obj); }
			// These are "advance primitive" data (special primitives)
			else if (&type == &Reflection::GetType<std::string>()) { writer.Write(*(const std::string*)obj); }
			else if (&type == &Reflection::GetType<std::wstring>()) { writer.Write(*(const std::wstring*)obj); }
			else if (&type == &Reflection::GetType<std::filesystem::path>()) { writer.Write(*(const std::filesystem::path*)obj); }
			else { SPX_CORE_ASSERT(false, "Primitive type not handled"); }
			return;
		}

		if (type.Kind == Reflection::TypeKind::ENUM)
		{
			const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(type);
			writer.Write(rEnum.GetName(obj));
			return;
		}

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(type);

		using CustomSerializer = ::Sphynx::Serialization::CustomSerializer<TWriter>;
		if (const CustomSerializer* serializer = rClass.GetAttribute<CustomSerializer>())
		{
			serializer->Write(obj, writer);
			return;
		}

		if (writer.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			// Treat Plain Old Data as directly copyable
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				writer.Write(obj, type.Size);
				return;
			}
		}
		using IndexedCollection = Reflection::CommonAttribute::IndexedCollection;
		using AssociativeCollection = Reflection::CommonAttribute::AssociativeCollection;
		if (const IndexedCollection* indexedCollection = rClass.GetAttribute<IndexedCollection>())
		{
			size_t collectionSize = indexedCollection->GetSize(obj);
			//writer.Write(collectionSize);
			if (!collectionSize) writer.PushFlow();

			writer.PushSequence();
			for (size_t i = 0; i < collectionSize; ++i)
			{
				ReflectionSerializer::Serialize(indexedCollection->Get(obj, i), indexedCollection->GetValueType(), writer); // NOTE: we are assuming no pointers
			}
			writer.PopSequence();
		}
		else if (const AssociativeCollection* associativeCollection = rClass.GetAttribute<AssociativeCollection>())
		{
			size_t collectionSize = associativeCollection->GetSize(obj);
			//writer.Write(collectionSize);

			if (collectionSize)
			{
				writer.PushMap();
				for (size_t i = 0; i < collectionSize; ++i)
				{
					// Serialize key
					{
						writer.PushKey();
						ReflectionSerializer::Serialize(associativeCollection->GetKey(obj, i), associativeCollection->GetKeyType(), writer);
					}

					// Serialize value
					{
						writer.PushValue();
						ReflectionSerializer::Serialize(associativeCollection->GetValue(obj, i), associativeCollection->GetValueType(), writer);
					}
				}
				writer.PopMap();
			}
		}
		else
		{
			// Write name and value for each property in the object
			writer.PushMap();
			for (const Reflection::Property& property : rClass)
			{
				// Write property name
				{
					writer.PushKey();
					writer.Write(property.Name);
				}

				// Write property content recursively
				{
					writer.PushValue();
					ReflectionSerializer::Serialize((const std::byte*)obj + property.Offset, property.GetType(), writer);
				}
			}
			writer.PopMap();
		}
	}
}