#include "TypedValue.h"

#include "Type.h"
#include "Registry.h"
#include "StdTypes.h"


namespace Sphynx
{
	namespace Reflection
	{
		TypedValue::TypedValue(const Type& type, const void* data) :
			m_Type(&type),
			m_Buffer(),
			m_HasValue(false)
		{
			if (data == nullptr) return;

			const bool fitsInBuffer = m_Type->Size <= sizeof(m_Buffer);
			if (fitsInBuffer)
			{
				m_Type->Construct(m_Buffer);
				if (data != nullptr)
				{
					std::memcpy(m_Buffer, data, m_Type->Size);
					m_HasValue = true;
				}
			}
			else
			{
				void* ptr = m_Type->Alloc();
				std::memcpy(m_Buffer, &ptr, sizeof(ptr));
				if (data != nullptr)
				{
					std::memcpy(ptr, data, m_Type->Size);
					m_HasValue = true;
				}
			}
		}

		TypedValue::TypedValue(TypedValue&& other) noexcept :
			m_Type(std::move(other.m_Type)),
			m_Buffer(),
			m_HasValue(std::move(other.m_HasValue))
		{
			std::memcpy(m_Buffer, other.m_Buffer, sizeof(m_Buffer));

			other.m_Type = nullptr;
			std::memset(other.m_Buffer, 0, sizeof(other.m_Buffer) / sizeof(decltype(*other.m_Buffer)));
			other.m_HasValue = false;
		}

		TypedValue::~TypedValue()
		{
			if (m_HasValue)
			{
				if (m_Type->Size <= sizeof(m_Buffer))
				{
					m_Type->Destruct(m_Buffer);
				}
				else
				{
					void* ptr = nullptr;
					std::memcpy(&ptr, m_Buffer, sizeof(ptr));
					m_Type->Dealloc(ptr);
				}
			}

			m_Type = nullptr;
			std::memset(m_Buffer, 0, sizeof(m_Buffer) / sizeof(decltype(*m_Buffer)));
			m_HasValue = false;
		}

		TypedValue TypedValue::Create(const Type& type, const void* data)
		{
			return TypedValue{ type, data };
		}

		TypedValue TypedValue::FromString(const char* str)
		{
			const size_t count = Registry::GetCount();
			for (size_t i = 0; i < count; ++i)
			{
				const Type* rType = Registry::TryGetTypeByIndex(i);
				if (rType == nullptr) continue;

				const Type& type = *rType;
				TypedValue tmpTypedValue{ type, nullptr };
				const bool fitsInBuffer = type.Size <= sizeof(tmpTypedValue.m_Buffer);
				void* buffer = fitsInBuffer ? (void*)tmpTypedValue.m_Buffer : (void*)&tmpTypedValue.m_Buffer;

				const bool result = type.FromString(buffer, str);
				if (result)
					return TypedValue{ *tmpTypedValue.m_Type, buffer };
			}

			SPX_CORE_ASSERT(false, "Cannot convert {} into a value", str);
			return TypedValue{ GetType<void>(), nullptr };
		}

		TypedValue TypedValue::FromString(const Type& type, const char* str)
		{
			TypedValue tmpTypedValue{ type, nullptr };
			const bool fitsInBuffer = type.Size <= sizeof(tmpTypedValue.m_Buffer);
			void* buffer = fitsInBuffer ? (void*)tmpTypedValue.m_Buffer : (void*)&tmpTypedValue.m_Buffer;
			const bool result = type.FromString(buffer, str);
			SPX_CORE_ASSERT(result, "Cannot convert {} into a value with type {}", str, type.Name);
			if (!result) return tmpTypedValue; // should hold nullptr

			return TypedValue{ *tmpTypedValue.m_Type, tmpTypedValue.GetValue() };
		}

		void TypedValue::ToString(const Type& type, const void* data, char* buffer, size_t size)
		{
			ToString(TypedValue{ type, data }, buffer, size);
		}

		void TypedValue::ToString(const TypedValue& typedValue, char* buffer, size_t size)
		{
			bool result = false;
			const bool fitsInBuffer = typedValue.m_Type->Size <= sizeof(typedValue.m_Buffer);
			void* buff = fitsInBuffer ? (void*)typedValue.m_Buffer : (void*)&typedValue.m_Buffer;

			if (!typedValue.m_Type->ToString(buff, buffer, size))
			{
				std::memset(buffer, 0, size);
			}
		}

		bool TypedValue::HasValue() const
		{
			return m_HasValue;
		}

		void* TypedValue::GetValue()
		{
			if (!m_HasValue) return nullptr;

			const bool fitsInBuffer = m_Type->Size <= sizeof(m_Buffer);
			void* buffer = fitsInBuffer ? (void*)m_Buffer : (void*)&m_Buffer;
			return buffer;
		}

		const void* TypedValue::GetValue() const
		{
			if (!m_HasValue) return nullptr;

			const bool fitsInBuffer = m_Type->Size <= sizeof(m_Buffer);
			void* buffer = fitsInBuffer ? (void*)m_Buffer : (void*)&m_Buffer;
			return buffer;
		}

		TypedValue& TypedValue::operator=(TypedValue&& other) noexcept
		{
			this->~TypedValue(); // release resources
			m_Type = other.m_Type;
			std::memcpy(m_Buffer, other.m_Buffer, sizeof(other.m_Buffer) / sizeof(decltype(*other.m_Buffer)));
			m_HasValue = other.m_HasValue;
			return *this;
		}

	}
}
