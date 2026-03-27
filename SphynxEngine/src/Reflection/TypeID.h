#pragma once

#include <cstdint>

namespace Sphynx
{
	namespace Reflection
	{
		constexpr size_t FNV1a_Hash(const char* str, size_t len) noexcept
		{
			constexpr size_t FNV_offset_basis = 14695981039346656037ULL;
			constexpr size_t FNV_prime = 1099511628211ULL;
			
			size_t hash = FNV_offset_basis;
			for (size_t i = 0; i < len; ++i)
			{
				hash ^= static_cast<unsigned char>(str[i]);
				hash *= FNV_prime;
			}
			return hash;
		}

		template<typename T>
		struct TypeID
		{
			constexpr static size_t ID = 0;
		};

	}
}

#define SPX_INTERNAL_REGISTER_TYPEID(_Type) \
    namespace Sphynx { namespace Reflection { \
    template<> struct TypeID<_Type> { \
        constexpr static size_t ID = FNV1a_Hash(#_Type, sizeof(#_Type) - 1); \
    }; }}
