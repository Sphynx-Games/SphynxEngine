#pragma once

#include "Core/Core.h"
#include <array>


namespace Sphynx
{
	struct UUID
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
			return hash<std::string>()(Sphynx::UUID::ToString(uuid));
		}
	};

}

