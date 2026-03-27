#pragma once

#include "Core/Core.h"
#include "Reflection/Attribute.h"
#include "Writer.h"
#include "Reader.h"


namespace Sphynx
{
	namespace Serialization
	{
		template<typename TWriter, typename T>
		void Write(TWriter& writer, const T& t);

		template<typename TReader, typename T>
		void Read(const TReader& reader, T& t);

		struct CustomSerializer : public ::Sphynx::Reflection::Attribute
		{
		public:
			template<typename T>
			CustomSerializer(Reflection::details::Tag<T>) :
				m_WriteFunc([](const void* obj, ::Sphynx::Writer& writer) {::Sphynx::Serialization::Write(writer, *(const T*)obj); })
			{}

			template<typename TWriter>
			void Write(const void* obj, TWriter& writer) const
			{
				::Sphynx::Writer w = ::Sphynx::Writer::Create(writer);
				m_WriteFunc(obj, w);
			}

			size_t GetTypeID() const override;

		private:
			void(*m_WriteFunc)(const void*, ::Sphynx::Writer&);
		};

		struct CustomDeserializer : public ::Sphynx::Reflection::Attribute
		{
		public:
			template<typename T>
			CustomDeserializer(Reflection::details::Tag<T>) :
				m_ReadFunc([](void* obj, const ::Sphynx::Reader& reader) {::Sphynx::Serialization::Read(reader, *(T*)obj); })
			{}

			template<typename TReader>
			void Read(void* obj, const TReader& reader) const
			{
				m_ReadFunc(obj, reader);
			}

			size_t GetTypeID() const override;

		private:
			void(*m_ReadFunc)(void*, const ::Sphynx::Reader&);
		};
	}
}

SPX_REGISTER_ATTRIBUTE(Sphynx::Serialization::CustomSerializer)
SPX_REGISTER_ATTRIBUTE(Sphynx::Serialization::CustomDeserializer)
