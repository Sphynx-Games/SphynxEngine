#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct Type;

		struct SPHYNX_API TypedValue
		{
		protected:
			TypedValue(const Type& type, const void* data);

		public:
			TypedValue(TypedValue&& other) noexcept;
			~TypedValue();

		public:
			template<typename T>
			static TypedValue Create(T&& value);
			static TypedValue Create(const Type& type, const void* data);

			static TypedValue FromString(const char* str);
			static TypedValue FromString(const Type& type, const char* str);
			static void ToString(const Type& type, const void* data, char* buffer, size_t size);
			static void ToString(const TypedValue& typedValue, char* buffer, size_t size);

			bool HasValue() const;
			void* GetValue();
			const void* GetValue() const;

			const Type* GetType() const;

			template<typename T>
			T& GetValue();
			template<typename T>
			const T& GetValue() const;

		public:
			TypedValue& operator=(TypedValue&& other) noexcept;

		private:
			const Type* m_Type;
			std::byte m_Buffer[sizeof(uintptr_t)];
			bool m_HasValue;

		};

		template<typename T>
		inline TypedValue TypedValue::Create(T&& value)
		{
			using TType = std::remove_cv_t<std::remove_reference_t<T>>;
			return Create(Reflection::GetType<TType>(), (const void*)&value);
		}

		template<typename T>
		inline T& TypedValue::GetValue()
		{
			// TODO: We don't have a conversion graph yet
			SPX_CORE_ASSERT(m_Type == &Reflection::GetType<T>(), "We don't support type conversion yet");
			void* value = GetValue();
			return *reinterpret_cast<T*>(value);
		}

		template<typename T>
		inline const T& TypedValue::GetValue() const
		{
			// TODO: We don't have a conversion graph yet
			SPX_CORE_ASSERT(m_Type == &Reflection::GetType<T>(), "We don't support type conversion yet");
			const void* value = GetValue();
			return *reinterpret_cast<const T*>(value);
		}
	}
}