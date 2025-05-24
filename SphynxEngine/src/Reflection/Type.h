#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	namespace Reflection
	{
		namespace details
		{
			template<typename> struct Tag;

			using TConstructFunction = void* (*)(void*);
			using TDestructFunction = void (*)(void*);
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
				m_DestructFunction(nullptr)
			{
				ConfigureType(details::Tag<T>{});
			}

		public:
			void* Construct(void* buffer) const
			{
				return m_ConstructFunction(buffer);
			}

			void Destruct(void* buffer) const
			{
				m_DestructFunction(buffer);
			}

			void* Alloc() const
			{
				return Construct(std::malloc(Size));
			}

			void Dealloc(void* buffer) const
			{
				Destruct(buffer);
				std::free(buffer);
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

		};
	}
}
