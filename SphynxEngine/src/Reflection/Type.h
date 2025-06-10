#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	namespace Reflection
	{
		namespace details
		{
			template<typename T, typename = void>
			struct is_from_string_function_defined : std::false_type {};

			template<typename T>
			struct is_from_string_function_defined<T, 
				std::void_t<decltype(
					::Sphynx::Reflection::FromString(std::declval<T&>(), std::declval<const char*>()))
				>> : std::true_type {};
		
			template<typename T, typename = void>
			struct is_to_string_function_defined : std::false_type {};

			template<typename T>
			struct is_to_string_function_defined<T, 
				std::void_t<decltype(
					::Sphynx::Reflection::ToString(std::declval<const T&>(), std::declval<char*>(), std::declval<size_t>()))
				>> : std::true_type {};
		}

		namespace details
		{
			template<typename> struct Tag;

			using TConstructFunction = void* (*)(void*);
			using TDestructFunction = void (*)(void*);
			using TFromStringFunction = bool(*)(void*, const char*);
			using TToStringFunction = bool(*)(const void*, char*, size_t);
		}

		enum class SPHYNX_API TypeKind
		{
			PRIMITIVE,
			ENUM,
			CLASS,
			STRUCT
		};

		struct SPHYNX_API Type
		{
		public:
			template<typename T>
			Type(details::Tag<T>, const char* name, size_t size, size_t alignment, TypeKind kind) :
				Name(name),
				Size(size),
				Alignment(alignment),
				Kind(kind),
				m_ConstructFunction(nullptr),
				m_DestructFunction(nullptr),
				m_FromStringFunction(nullptr),
				m_ToStringFunction(nullptr)
			{
				ConfigureType(details::Tag<T>{});
			}

		public:
			void* Construct(void* buffer) const
			{
				if (m_ConstructFunction)
				{
					return m_ConstructFunction(buffer);
				}

				return nullptr;
			}

			void Destruct(void* buffer) const
			{
				if (m_DestructFunction)
				{
					m_DestructFunction(buffer);
				}
			}

			void* Alloc() const
			{
				if (!m_ConstructFunction) return nullptr;

				return Construct(std::malloc(Size));
			}

			void Dealloc(void* buffer) const
			{
				if (!m_DestructFunction) return;

				Destruct(buffer);
				std::free(buffer);
			}

			bool FromString(void* buffer, const char* str) const
			{
				if (m_FromStringFunction)
				{
					return m_FromStringFunction(buffer, str);
				}
				return false;
			}

			bool ToString(const void* buffer, char* str, size_t size) const
			{
				if (m_ToStringFunction)
				{
					return m_ToStringFunction(buffer, str, size);
				}
				return false;
			}

		private:
			template<typename T>
			void ConfigureType(details::Tag<T>)
			{
				if constexpr (std::is_default_constructible_v<T>)
				{
					m_ConstructFunction = [](void* buffer) -> void* { return new(buffer) T{}; };
				}
				if constexpr (std::is_destructible_v<T>)
				{
					m_DestructFunction = [](void* buffer) -> void { static_cast<T*>(buffer)->~T(); };
				}

				if constexpr (details::is_from_string_function_defined<T>::value)
				{
					m_FromStringFunction = [](void* buffer, const char* str) -> bool
						{
							return ::Sphynx::Reflection::FromString(*static_cast<T*>(buffer), str);
						};
				}
				if constexpr (details::is_to_string_function_defined<T>::value)
				{
					m_ToStringFunction = [](const void* buffer, char* str, size_t size) -> bool
						{
							return ::Sphynx::Reflection::ToString(*static_cast<const T*>(buffer), str, size);
						};
				}
			}

		public:
			const char* Name;
			size_t Size;
			size_t Alignment;
			TypeKind Kind;

		private:
			// Utility function pointers
			details::TConstructFunction m_ConstructFunction;
			details::TDestructFunction m_DestructFunction;

			details::TFromStringFunction m_FromStringFunction;
			details::TToStringFunction m_ToStringFunction;

		};
	}
}
