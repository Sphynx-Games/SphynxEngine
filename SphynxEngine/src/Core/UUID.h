#pragma once

#include "Core/Core.h"
#include <array>


namespace Sphynx
{
	struct SPHYNX_API UUID
	{
	public:
		UUID();
		UUID(const UUID& other);

	public:
		static UUID Generate();
		static UUID FromString(const std::string& str);
		static std::string ToString(const UUID& uuid);

	private:
		friend bool operator==(const UUID& lhs, const UUID& rhs);

	private:
		std::array<uint8_t, 16> m_Data;

	public:
		static const UUID Invalid;

    private:
        friend struct std::hash<UUID>;

	};
}

namespace std 
{
	template <typename T> struct hash;

	template<>
	struct hash<Sphynx::UUID>
	{
		std::size_t operator()(const Sphynx::UUID& uuid) const
		{
            uint64_t l =
                static_cast<uint64_t>(uuid.m_Data[0]) << 56 |
                static_cast<uint64_t>(uuid.m_Data[1]) << 48 |
                static_cast<uint64_t>(uuid.m_Data[2]) << 40 |
                static_cast<uint64_t>(uuid.m_Data[3]) << 32 |
                static_cast<uint64_t>(uuid.m_Data[4]) << 24 |
                static_cast<uint64_t>(uuid.m_Data[5]) << 16 |
                static_cast<uint64_t>(uuid.m_Data[6]) << 8 |
                static_cast<uint64_t>(uuid.m_Data[7]);
            uint64_t h =
                static_cast<uint64_t>(uuid.m_Data[8]) << 56 |
                static_cast<uint64_t>(uuid.m_Data[9]) << 48 |
                static_cast<uint64_t>(uuid.m_Data[10]) << 40 |
                static_cast<uint64_t>(uuid.m_Data[11]) << 32 |
                static_cast<uint64_t>(uuid.m_Data[12]) << 24 |
                static_cast<uint64_t>(uuid.m_Data[13]) << 16 |
                static_cast<uint64_t>(uuid.m_Data[14]) << 8 |
                static_cast<uint64_t>(uuid.m_Data[15]);

            if constexpr (sizeof(size_t) > 4)
            {
                return size_t(l ^ h);
            }
            else
            {
                uint64_t hash64 = l ^ h;
                return size_t(uint32_t(hash64 >> 32) ^ uint32_t(hash64));
            }
		}
	};

}

