#pragma once

#include "Core/Core.h"
#include "Reflection/Reflection.h"


namespace Sphynx
{
	namespace Serialization
	{
		template<typename TWriter, typename T>
		void Write(TWriter& writer, const T& t);

		template<typename TReader, typename T>
		void Read(const TReader& reader, T& t);

		template<typename TWriter>
		struct CustomSerializer : public ::Sphynx::Reflection::Attribute
		{
		public:
			template<typename T>
			CustomSerializer(Reflection::details::Tag<T>) :
				m_WriteFunc([](const void* obj, TWriter& writer) {::Sphynx::Serialization::Write(writer, *(const T*)obj); })
			{}

			void Write(const void* obj, TWriter& writer) const
			{
				m_WriteFunc(obj, writer);
			}

		private:
			void(*m_WriteFunc)(const void*, TWriter&);
		};

		template<typename TReader>
		struct CustomDeserializer : public ::Sphynx::Reflection::Attribute
		{
		public:
			template<typename T>
			CustomDeserializer(Reflection::details::Tag<T>) :
				m_ReadFunc([](void* obj, const TReader& reader) {::Sphynx::Serialization::Read(reader, *(T*)obj); })
			{}

			void Read(void* obj, const TReader& reader) const
			{
				m_ReadFunc(obj, reader);
			}

		private:
			void(*m_ReadFunc)(void*, const TReader&);
		};
	}
}
